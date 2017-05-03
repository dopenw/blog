#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <unistd.h>

using namespace std;

bool exists_file_y_n (const std::string& name) {
        ifstream f(name.c_str());
        return f.good();
}

int getPosFromVector(const string config_file,std::vector<string> vec)
{
        auto it = std::find(vec.begin(), vec.end(), config_file);
        if (it == vec.end())
        {
                return -1;
        } else
        {
                auto index = std::distance(vec.begin(), it);
                return index;
        }

}

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


void write_base_markdown(const string config_file,std::vector<string> &vec,const string Categories)
{
        ofstream outfile(config_file);
        outfile<<"# Categories"<<" "<<Categories<<endl;
        outfile<<"* ## [home](../README.md)"<<endl;
        for(unsigned int i=0; i<(vec.size()/2); i++)
        {
                outfile<<"* ### "<<'['<< vec[i*2]<<']'<<'('<< vec[i*2+1]<<')'<<endl;
        }
        outfile<<"                           step by steop";
        // vec.clear();
}
void clear_blog_tail_links(const string blog_file)
{
        ifstream infile(blog_file.c_str());
        ofstream outfile("tmp.md");
        string tmp_line;
        while (getline(infile,tmp_line)) {
                if (tmp_line=="[上一级](base.md)")
                        break;
                outfile<<tmp_line<<std::endl;
        }
        remove(blog_file.c_str());
        rename("tmp.md",blog_file.c_str());
}

int blog_add_pre_next_links(const string blog_file,std::vector<string> &vec)
{
        if (exists_file_y_n(blog_file))
        {
                clear_blog_tail_links(blog_file);
                ofstream outfile(blog_file,std::ios::app);
                outfile<<std::endl<<"[上一级](base.md)"<<endl;
                auto pos=getPosFromVector(blog_file,vec);
                if(pos==-1)
                {
                        cout<<"error ,get pos from vector is wrong";
                        return -1;
                }
                if(pos==1)
                {
                        if(pos<vec.size()-1)
                                outfile<<"[下一篇]"<<"("<< vec[pos+2] <<")"<<endl;
                }
                else
                {
                        if(pos==vec.size()-1)
                        {
                                outfile<<"[上一篇]"<<"("<< vec[pos-2] <<")"<<endl;
                        }
                        if(pos<vec.size()-1)
                        {
                                outfile<<"[上一篇]"<<"("<< vec[pos-2] <<")"<<endl;
                                outfile<<"[下一篇]"<<"("<< vec[pos+2] <<")"<<endl;
                        }
                }


        }
        return 0;
}

int main(int argc, char const *argv[]) {
        std::vector<string> global;
        std::vector<string> vec;
        read_config("global",global);
        const char *tmp=nullptr;
        for (unsigned int path_count=0; path_count<global.size(); path_count++)
        {
                tmp=("./"+global[path_count]).c_str();
                if(chdir(tmp)<0)
                        std::cout << "chdir error" << '\n';
                read_config(global[path_count],vec);
                write_base_markdown("base.md",vec,global[path_count]);
                for(unsigned int write_config_link_count=0; write_config_link_count<vec.size()/2; write_config_link_count++)
                {
                        if(blog_add_pre_next_links(vec[write_config_link_count*2+1],vec)==-1)
                        {
                                std::cout << "error ,blog_add_pre_next_links is wrong" << '\n';
                        }
                }
                vec.clear();
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
