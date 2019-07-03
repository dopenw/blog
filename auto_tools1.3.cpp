#define _DEFAULT_SOURCE
#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <regex>
#include <string>
#include <time.h>
#include <unistd.h>
#include <vector>
using namespace std;

bool exists_file_y_n(const std::string &name) {
  ifstream f(name.c_str());
  return f.good();
}

void read_config(const string config_file, std::vector<string> &vec) {
  ifstream file(config_file);
  string tmp_string;
  if (file.is_open()) {
    while (!file.eof()) {
      file >> tmp_string;
      if (!tmp_string.empty())
        vec.push_back(tmp_string);
    }
    if (!vec.empty()) {
      vec.pop_back();
    }
  }
}

bool list_dir(std::vector<string> &blog_file, std::string list_path) {

  struct dirent **namelist;
  int n;

  n = scandir(list_path.c_str(), &namelist, NULL, versionsort);
  if (n == -1) {
    perror("scandir");
    return false;
  }

  for (int i = 0; i < n; i++) {
    if (strcmp(namelist[i]->d_name, ".") == 0 ||
        strcmp(namelist[i]->d_name, "..") == 0 ||
        strcmp(namelist[i]->d_name, "README.md") == 0) {
      free(namelist[i]);
      continue;
    }
    blog_file.push_back(namelist[i]->d_name);
    free(namelist[i]);
  }
  free(namelist);

  return true;
}

const std::string currentDateTime() {
  time_t now = time(0);
  struct tm *tstruct;
  char buf[80];
  tstruct = localtime(&now);
  strftime(buf, sizeof(buf), "%Y%m%d%H%M", tstruct);
  return buf;
}

bool regex_match_replace_img(std::string number, std::string currentTime,
                             std::string blog_file_name) {
  std::string blog_name = blog_file_name.substr(0, blog_file_name.size() - 3);
  std::string blog_file_img_name =
      blog_name + "_" + currentTime + "_" + number + ".png";
  std::string shutter_default_name = "Selection_00";
  std::string src_img_path =
      "/home/breap/Pictures/" + shutter_default_name + number + ".png";
  std::string des_img_path = "../images/" + blog_file_img_name;
  int result = 0;
  result = rename(src_img_path.c_str(), des_img_path.c_str());
  if (result == 0) {
    puts("File successfully renamed");
    return true;
  }

  else {
    perror("Error renaming file");
    return false;
  }
}

void write_base_markdown(const string config_file,
                         std::vector<pair<string, string>> vec_map_blog,
                         const string Categories) {
  ofstream outfile(config_file);
  outfile << "# Categories"
          << " " << Categories << endl;
  outfile << "* ## [home](../README.md)" << endl;
  for (auto i = vec_map_blog.begin(); i != vec_map_blog.end(); ++i) {
    outfile << "* ### " << '[' << i->second << ']' << '(' << i->first << ')'
            << endl;
  }
  outfile << "                           step by steop";
}

void open_blog_clear_tail_links(
    const string blog_file, std::vector<pair<string, string>> &vec_map_blog) {
  std::regex reg1("(.*#.*)");
  std::regex reg2("[^# ].+");
  std::regex reg3(".*addimage.*\\d+");
  std::regex reg4("\\d+");
  std::cmatch cm;
  ifstream infile(blog_file.c_str());
  ofstream outfile("tmp.md");
  string tmp_line;
  bool found;
  int only_get_one_blog_name = 0;
  while (getline(infile, tmp_line)) {
    found = regex_match(tmp_line.c_str(), reg1);
    if (found && (only_get_one_blog_name++) == 0) {
      regex_search(tmp_line.c_str(), cm, reg2);
      string tmp = cm[0];
      vec_map_blog.push_back(make_pair(blog_file, tmp));
      outfile << tmp_line << std::endl;
      continue;
    }
    found = regex_match(tmp_line.c_str(), reg3);
    if (found) {
      std::string currentTime = currentDateTime();
      regex_search(tmp_line.c_str(), cm, reg4);
      if (regex_match_replace_img(cm[0], currentTime, blog_file)) {
        string number = cm[0];
        outfile << "![]"
                << "("
                << "../images/" + blog_file.substr(0, blog_file.size() - 3) +
                       "_" + currentTime + "_" + number + ".png"
                << ")" << endl;
        continue;
      }
    }
    if (tmp_line == "[上一级](README.md)")
      break;

    outfile << tmp_line << std::endl;
  }
  remove(blog_file.c_str());
  rename("tmp.md", blog_file.c_str());
}

int blog_add_pre_next_links(
    const string blog_file,
    const std::vector<pair<string, string>> vec_map_blog) {
  if (exists_file_y_n(blog_file)) {
    ofstream outfile(blog_file, std::ios::app);
    outfile << "[上一级](README.md)" << endl;

    auto pos = find_if(vec_map_blog.begin(), vec_map_blog.end(),
                       [=](pair<string, string> file_title) {
                         if (file_title.first == blog_file)
                           return true;
                         else
                           return false;
                       });
    if (pos == vec_map_blog.end()) {
      cout << "error ,not found blog_file";
      return -1;
    }

    if (pos == vec_map_blog.begin()) {
      auto tmp = pos;
      if ((++tmp) != vec_map_blog.end()) {
        outfile << "[下一篇]"
                << "(" << (tmp)->first << ")" << endl;
      }
    } else {
      auto tmp = pos;
      auto tmp_end = vec_map_blog.end();
      if ((++tmp) == (tmp_end)) {
        tmp = pos;
        outfile << "[上一篇]"
                << "(" << (--tmp)->first << ")" << endl;
      }
      tmp = pos;
      if ((++tmp) != (vec_map_blog.end())) {
        tmp = pos;
        outfile << "[上一篇]"
                << "(" << (--tmp)->first << ")" << endl;
        tmp = pos;
        outfile << "[下一篇]"
                << "(" << (++tmp)->first << ")" << endl;
      }
    }
  }
  return 0;
}

int main(int argc, char const *argv[]) {
  std::vector<string> global;
  std::vector<string> vec;
  read_config("global", global);
  std::vector<pair<string, string>> vec_map_blog;
  std::string blog_root_path;
  blog_root_path = get_current_dir_name();
  if (exists_file_y_n(blog_root_path + "/global")) {
    for (unsigned int path_count = 0; path_count < global.size();
         path_count++) {
      if (chdir((blog_root_path + "/" + global[path_count]).c_str()) < 0) {
        std::cout << "chdir error" << '\n';
        perror("chdir error:");
        return -1;
      }
      list_dir(vec, ".");

      for (unsigned int write_config_link_count = 0;
           write_config_link_count < vec.size(); write_config_link_count++) {
        open_blog_clear_tail_links(vec[write_config_link_count], vec_map_blog);
      }

      for (unsigned int write_config_link_count = 0;
           write_config_link_count < vec.size(); write_config_link_count++) {
        if (blog_add_pre_next_links(vec[write_config_link_count],
                                    vec_map_blog) == -1) {
          std::cout << "error ,blog_add_pre_next_links is wrong" << '\n';
        }
      }

      vec.clear();
      write_base_markdown("README.md", vec_map_blog, global[path_count]);
      vec_map_blog.clear();

      if (chdir(blog_root_path.c_str()) < 0)
        std::cout << "chdir error" << '\n';
    }
  }

  std::vector<string> v_tmp;
  std::string pictures_home = "/home/breap/Pictures/";
  list_dir(v_tmp, pictures_home);
  std::regex reg_del_image("Selection_.*");

  for (auto image_file : v_tmp) {
    if (regex_match(image_file.c_str(), reg_del_image)) {
      int status = unlink((pictures_home + image_file).c_str());
      if (status == 0) {
        std::cout << "del_image:  " << image_file << '\n';
      } else
        std::cout << "delete image file is failed" << '\n';
    }
    // std::cout << "image_file"<<image_file << '\n';
  }
  return 0;
}
