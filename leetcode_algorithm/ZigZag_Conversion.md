# 折线转换(zigzag)

he string "PAYPALISHIRING" is written in a zigzag pattern on a given number of rows like this: (you may want to display this pattern in a fixed font for better legibility)

```
P   A   H   N
A P L S I I G
Y   I   R
```
And then read line by line: "PAHNAPLSIIGYIR"
Write the code that will take a string and make this conversion given a number of rows:

```C
string convert(string text, int nRows);
```
convert("PAYPALISHIRING", 3) should return "PAHNAPLSIIGYIR"


The problem statement itself is unclear for many. Especially for 2-row case. "ABCD", 2 --> "ACBD". The confusion most likely is from the character placement. I would like to extend it a little bit to make ZigZag easy understood.

The example can be written as follow:

```
P.......A........H.......N
..A..P....L..S....I...I....G
....Y.........I........R
```
Therefore, <ABCD, 2> can be arranged as:

```
A....C
...B....D
```
[algorithm link](https://leetcode.com/problems/zigzag-conversion/discuss/)

```c++
string convert(string s, int nRows) {

    if (nRows <= 1)
        return s;

    const int len = (int)s.length();
    string *str = new string[nRows];

    int row = 0, step = 1;
    for (int i = 0; i < len; ++i)
    {
        str[row].push_back(s[i]);

        if (row == 0)
            step = 1;
        else if (row == nRows - 1)
            step = -1;

        row += step;
    }

    s.clear();
    for (int j = 0; j < nRows; ++j)
    {
        s.append(str[j]);
    }

    delete[] str;
    return s;
}
```
















[source link](https://leetcode.com/problems/zigzag-conversion/discuss/)







[上一级](base.md)
[上一篇](Swap_Nodes_in_Pairs.md)
[下一篇](add_binary.md)
