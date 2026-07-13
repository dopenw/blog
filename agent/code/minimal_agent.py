import argparse
import ast
import json
import math
import operator
import os
import time
from datetime import datetime
from pathlib import Path
from zoneinfo import ZoneInfo

from dotenv import load_dotenv
from openai import APIError, APIStatusError, APITimeoutError, OpenAI
# 从 code/.env 读取本地配置；已有的系统环境变量不会被覆盖。
load_dotenv(Path(__file__).with_name(".env"))
MODEL = os.getenv("MODEL", "gpt-4o-mini")
MAX_STEPS = int(os.getenv("AGENT_MAX_STEPS", "6"))
TOTAL_TIMEOUT_SECONDS = float(os.getenv("AGENT_TOTAL_TIMEOUT", "60"))
REQUEST_TIMEOUT_SECONDS = float(os.getenv("OPENAI_API_TIMEOUT", "20"))
VERBOSE = os.getenv("VERBOSE", "true").lower() in {"1", "true", "yes"}
SYSTEM_PROMPT = "你是简洁、准确的中文助手。根据工具结果直接回答；不用 Markdown 标题、LaTeX、表格或表情符号。数学式使用普通文本，例如“(18 + 6) × 3 = 72”。"
TOOLS = [
    {
        "type": "function",
        "function": {
            "name": "calculate",
            "description": "计算一个只含数字与 + - * / ** 括号的数学表达式。",
            "parameters": {
                "type": "object",
                "properties": {"expression": {"type": "string", "description": "例如 (18 + 6) * 3"}},
                "required": ["expression"],
                "additionalProperties": False,
            },
        },
    },
    {
        "type": "function",
        "function": {
            "name": "get_current_time",
            "description": "获取指定 IANA 时区的当前时间；默认 Asia/Shanghai。",
            "parameters": {
                "type": "object",
                "properties": {"timezone": {"type": "string", "description": "例如 Asia/Shanghai"}},
                "required": ["timezone"],
                "additionalProperties": False,
            },
        },
    },
]
OPERATORS = {
    ast.Add: operator.add,
    ast.Sub: operator.sub,
    ast.Mult: operator.mul,
    ast.Div: operator.truediv,
    ast.Pow: operator.pow,
    ast.USub: operator.neg,
    ast.UAdd: operator.pos,
}
def calculate(expression: str) -> float:
    """用 AST 白名单计算表达式，绝不对用户输入使用 eval。"""
    if len(expression) > 100:
        raise ValueError("表达式过长")
    def visit(node):
        if isinstance(node, ast.Constant) and type(node.value) in (int, float):
            return node.value
        if isinstance(node, ast.UnaryOp) and type(node.op) in OPERATORS:
            return OPERATORS[type(node.op)](visit(node.operand))
        if isinstance(node, ast.BinOp) and type(node.op) in OPERATORS:
            left, right = visit(node.left), visit(node.right)
            if isinstance(node.op, ast.Pow) and abs(right) > 10:
                raise ValueError("指数绝对值不能超过 10")
            return OPERATORS[type(node.op)](left, right)
        raise ValueError("只允许数字、+、-、*、/、** 和括号")
    value = visit(ast.parse(expression, mode="eval").body)
    if not math.isfinite(value):
        raise ValueError("计算结果不是有限数字")
    return value
def get_current_time(timezone: str) -> str:
    return datetime.now(ZoneInfo(timezone)).isoformat(timespec="seconds")
FUNCTIONS = {"calculate": calculate, "get_current_time": get_current_time}
def log(message: str) -> None:
    if VERBOSE:
        print(message)
def execute_tool(name: str, arguments_json: str) -> str:
    """解析模型的 JSON 参数、调用白名单工具，并统一返回 JSON 字符串。"""
    try:
        arguments = json.loads(arguments_json)
        function = FUNCTIONS.get(name)
        if function is None:
            raise ValueError(f"未知工具：{name}")
        return json.dumps({"ok": True, "result": function(**arguments)}, ensure_ascii=False)
    except (json.JSONDecodeError, TypeError, ValueError, ZeroDivisionError) as error:
        return json.dumps({"ok": False, "error": str(error)}, ensure_ascii=False)
def run_agent(question: str) -> str:
    if not os.getenv("OPENAI_API_KEY"):
        raise RuntimeError("请先设置环境变量 OPENAI_API_KEY")
    base_url = os.getenv("OPENAI_BASE_URL")
    client = OpenAI(
        base_url=base_url,
        timeout=REQUEST_TIMEOUT_SECONDS,
        max_retries=1,
    )
    messages = [
        {"role": "system", "content": SYSTEM_PROMPT},
        {"role": "user", "content": question},
    ]
    deadline = time.monotonic() + TOTAL_TIMEOUT_SECONDS
    log(f"开始执行 Agent：模型={MODEL}，最大步数={MAX_STEPS}")
    for step in range(1, MAX_STEPS + 1):
        if time.monotonic() >= deadline:
            raise TimeoutError(f"Agent 超过总超时 {TOTAL_TIMEOUT_SECONDS:g} 秒")
        try:
            log(f"\n[第 {step} 步] 请求模型决定下一步…")
            response = client.chat.completions.create(model=MODEL, messages=messages, tools=TOOLS)
        except APITimeoutError as error:
            raise TimeoutError(f"第 {step} 步 API 请求超时") from error
        except APIStatusError as error:
            raise RuntimeError(
                f"第 {step} 步 API 调用失败：HTTP {error.status_code}；模型={MODEL}，地址={base_url}"
            ) from error
        except APIError as error:
            raise RuntimeError(f"第 {step} 步 API 调用失败：{error}") from error

        message = response.choices[0].message
        messages.append(message)
        calls = message.tool_calls or []
        if not calls:
            log(f"[第 {step} 步] 模型不再调用工具，准备输出最终答案。")
            return message.content or "模型没有返回最终文本。"

        for call in calls:
            log(f"[第 {step} 步] 模型选择工具：{call.function.name}")
            log(f"  参数：{call.function.arguments}")
            tool_result = execute_tool(call.function.name, call.function.arguments)
            log(f"  工具结果：{tool_result}")
            messages.append({
                "role": "tool",
                "tool_call_id": call.id,
                "content": tool_result,
            })

    raise RuntimeError(f"Agent 超过最大步数 {MAX_STEPS}，已停止以避免无限循环")
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="最小 OpenAI 工具调用 Agent")
    parser.add_argument("question", nargs="?", default="现在上海几点？并计算 (18 + 6) * 3。")
    args = parser.parse_args()
    try:
        print(run_agent(args.question))
    except (RuntimeError, TimeoutError) as error:
        raise SystemExit(f"Agent 失败：{error}") from error
