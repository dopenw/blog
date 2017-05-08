#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <dirent.h>
#include <map>
#include <regex>
#include <iterator>
using namespace std;

bool exists_file_y_n (const std::string& name) {
        ifstream f(name.c_str());
        return f.good();
}

// int getPosFromVector(const string config_file,std::vector<string> vec)
// {
//         auto it = std::find(vec.begin(), vec.end(), config_file);
//         if (it == vec.end())
//         {
//                 return -1;
//         } else
//         {
//                 auto index = std::distance(vec.begin(), it);
//                 return index;
//         }
//
// }
//
void read_config(const string config_file,std::vector<string> &vec)
{
        ifstream file(config_file);
        string tmp_string;
        if(file.is_open())
        {
                while (!file.eof())
                {
                        file >>tmp_string;
                        vec.push_back(tmp_string);
                }
                if(!vec.empty())
                {
                        vec.pop_back();
                }
        }
        for(auto i:vec)
        {
                std::cout << i << '\n';
        }
}

bool list_dir(std::vector<string> &blog_file)
{
        DIR *dp;
        struct dirent *dirp;
        if((dp=opendir("."))==NULL)
        {
                std::cout << "opendir error" << '\n';
                return false;
        }

        while ((dirp=readdir(dp))!=NULL)
        {

                if(strcmp(dirp->d_name,".")==0 ||
                   strcmp(dirp->d_name,"..")==0 ||
                   strcmp(dirp->d_name,"base.md")==0
                   )
                        continue;
                std::cout << dirp->d_name << '\n';
                blog_file.push_back(dirp->d_name);
        }

        if(closedir(dp)<0)
                std::cout << "close direcory failed" << '\n';
        return true;
}


bool regex_match_replace_img(std::string number,std::string blog_file_name)
{
        std::string blog_name=blog_file_name.substr(0,blog_file_name.size()-3);
        std::string blog_file_img_name=blog_name+number+".png";
        std::string src_img_path="/home/ghi/Pictures/"+number+".png";
        std::string des_img_path="./images/"+blog_file_img_name;
        int result=0;
        result= rename( src_img_path.c_str(),des_img_path.c_str());
        if ( result == 0 )
        {
                puts ( "File successfully renamed" );
                return true;
        }

        else
        {
                perror( "Error renaming file" );
                return false;
        }

}


void write_base_markdown(const string config_file,std::map<string, string> map_blog,const string Categories)
{
        ofstream outfile(config_file);
        outfile<<"# Categories"<<" "<<Categories<<endl;
        outfile<<"* ## [home](../README.md)"<<endl;
        for(auto i=map_blog.begin();i!=map_blog.end();++i)
        {
                outfile<<"* ### "<<'['<< i->second <<']'<<'('<< i->first<<')'<<endl;
        }
        outfile<<"                           step by steop";
        // vec.clear();
}
void open_blog_clear_tail_links(const string blog_file,std::map<string, string> &map_blog)
{
        std::regex reg1("(.*#.*)");
        std::regex reg2("[^# ].+");
        std::regex reg3(".*addimage.*\\d+");
        std::regex reg4("\\d+");
        std::cmatch cm;
        bool rm_other_digtal_image=false;
        ifstream infile(blog_file.c_str());
        ofstream outfile("tmp.md");
        string tmp_line;
        bool found;
        int only_get_one_blog_name=0;
        while (getline(infile,tmp_line)) {
                found=regex_match(tmp_line.c_str(),reg1);
                if(found && only_get_one_blog_name==0)
                {
                  only_get_one_blog_name=1;
                        regex_search(tmp_line.c_str(),cm,reg2);
                        string tmp=cm[0];
                        map_blog.insert(make_pair(blog_file,tmp));
                        outfile<<tmp_line<<std::endl;
                        continue;
                }
                found=regex_match(tmp_line.c_str(),reg3);
                if(found)
                {
                        regex_search(tmp_line.c_str(),cm,reg4);
                        regex_match_replace_img(cm[0],blog_file);
                        string number=cm[0];
                        outfile<<"![../images]"<<"("<<blog_file+number+".png"<<")"<<endl;
                        continue;
                        rm_other_digtal_image=true;
                }
                if (tmp_line=="[上一级](base.md)")
                        break;

                outfile<<tmp_line<<std::endl;
        }
        remove(blog_file.c_str());
        rename("tmp.md",blog_file.c_str());
        if(rm_other_digtal_image==true)
        {
          cout<<"delete other number.png"<<endl;
        }
}

int blog_add_pre_next_links(const string blog_file,const std::map<string, string> map_blog)
{
        if (exists_file_y_n(blog_file))
        {
                ofstream outfile(blog_file,std::ios::app);
                outfile<<std::endl<<"[上一级](base.md)"<<endl;
                for(auto i:map_blog)
                {
                  std::cout << i.first<<":"<<i.second << '\n';
                }
                auto pos=map_blog.find(blog_file);
                if(pos==map_blog.end())
                {
                        cout<<"error ,not found blog_file";
                        return -1;
                }
                if(pos==map_blog.begin())
                {
                        if((++pos)!=map_blog.end())
                                outfile<<"[下一篇]"<<"("<< (++pos)->first <<")"<<endl;
                }
                else
                {
                        if(pos==(--map_blog.end()))
                        {
                                outfile<<"[上一篇]"<<"("<< (--pos)->first <<")"<<endl;
                        }
                        if((++pos)!=(map_blog.end()))
                        {
                                outfile<<"[上一篇]"<<"("<< (--pos)->first <<")"<<endl;
                                outfile<<"[下一篇]"<<"("<< (++pos)->first <<")"<<endl;
                        }
                }


        }
        return 0;
}

int main(int argc, char const *argv[]) {
        std::vector<string> global;
        std::vector<string> vec;
        read_config("global",global);
        std::map<string, string> map_blog;
        const char *tmp=nullptr;
        for (unsigned int path_count=0; path_count<global.size(); path_count++)
        {
                tmp=("./"+global[path_count]).c_str();
                if(chdir(tmp)<0)
                        std::cout << "chdir error" << '\n';
                list_dir(vec);

                for(unsigned int write_config_link_count=0; write_config_link_count<vec.size(); write_config_link_count++)
                {
                  open_blog_clear_tail_links(vec[write_config_link_count],map_blog);
                        if(blog_add_pre_next_links(vec[write_config_link_count],map_blog)==-1)
                        {
                                std::cout << "error ,blog_add_pre_next_links is wrong" << '\n';
                        }
                }

                vec.clear();
                write_base_markdown("base.md",map_blog,global[path_count]);
                map_blog.clear();

                tmp="../";
                if(chdir(tmp)<0)
                        std::cout << "chdir error" << '\n';
        }
        for(auto i:vec)
        {
                std::cout << i << '\n';
        }



        return 0;
}
