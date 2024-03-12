#include"../../include/net/HttpContext.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<string>
#include<stdio.h>
#include<string.h>
#include<mysql/mysql.h>
using namespace fei;

HttpContext::ParseUrlRet HttpContext::parse_Url()
{   //   1            2
    //GET /(index.html) HTTP1.1\r\n
    size_t endl_pos= instr_.find('\n', 0);
    if(endl_pos == instr_.npos) 
    {
        // not find endl, we need more data to parse
        return ParseUrlRet::PARSE_AGAIN;
    }
    else 
    {
        size_t get_pos = instr_.find("GET");
        size_t post_pos = instr_.find("POST");

        if(get_pos != instr_.npos)
            method_ = Method::GET;
        else if(post_pos != instr_.npos)
            method_ = Method::POST;
        // else 
        //     return ParseUrlRet::PARSE_ERROR;
        
        size_t url_pos = instr_.find('/', 0);   // find the second space
        if(url_pos != instr_.npos)
        {
            size_t space_pos = instr_.find(' ', url_pos);//the pos of the second space
            url_raw_path_ = instr_.substr(url_pos, space_pos - url_pos);
            if(url_raw_path_.back() == '/') url_raw_path_.append("index.html");
            
            //url_raw_path_
            
            size_t version_end = instr_.find('\r', space_pos);
            std::string version = instr_.substr(space_pos + 1, version_end - space_pos - 1);
            if(version == "HTTP/1.1")
                version_ = Version::HTTP11;
            else if(version == "HTTP/1.0")
                version_ = Version::HTTP10;

            if(endl_pos + 1 < instr_.size())
                instr_ = instr_.substr(endl_pos + 1, instr_.size() - endl_pos - 1);
            else 
                instr_.clear();

            return ParseUrlRet::PARSE_SUCCESS;
        }
        else 
        {
            return ParseUrlRet::PARSE_ERROR;
        }

    }
}
    //GET /(index.html) HTTP1.1\r\n
HttpContext::ParseHeadersRet HttpContext::parse_headers()
{
    //key: value\r\n
    size_t empty_line_pos = instr_.find("\r\n\r\n", 0);
    if(empty_line_pos != instr_.npos)
    {
        //ok, we now get the headers completely
        size_t key_pos = 0;
        size_t split_pos = 0;
        size_t endl_pos  = 0;
        do
        {
            split_pos = instr_.find(':',  key_pos);
            endl_pos = instr_.find("\r\n", key_pos);

            if(split_pos == instr_.npos && endl_pos != instr_.npos)
            {
                //we get the empty line
                //key1: value2\r\n
                //key2: value2\r\n
                //\r\n
                //instr_ = instr_.substr(0, endl_pos + 2);
                if(endl_pos + 2 < instr_.size())
                    instr_ = instr_.substr(endl_pos + 2, instr_.size() - endl_pos - 2);
                else 
                    instr_.clear();
                return ParseHeadersRet::PARSE_SUCCESS;
            }

            headers.insert({instr_.substr(key_pos, split_pos - key_pos),
                            instr_.substr(split_pos + 1, endl_pos - split_pos - 1)});

            key_pos = endl_pos + 2;
        } while (endl_pos != instr_.npos);
        assert(0);
    }
    else
    {
        //FIXME: maybe we can parse the data we have gotten?

        return ParseHeadersRet::PARSE_AGAIN;
    } 


}

HttpContext::ParseBodysRet HttpContext::parse_body()
{
    //we should get the "Content-Length"
    size_t content_length = std::stoul(headers["Content-Length"]);
    if(instr_.size() < content_length)
    {
        return HttpContext::ParseBodysRet::PARSE_AGAIN;
    }
    else 
    {
        //assert(instr_.size() == content_length);
        body_ = instr_.substr(0, content_length);
        if(content_length == instr_.size())
            //instr_ = instr_.substr(content_length - 1, instr_.size() - content_length - 1);
            instr_.clear();
        else 
            instr_ = instr_.substr(content_length, instr_.size() - content_length);

        //user=123&password=456&gender=male&age=12
        size_t and_pos = 0;
        size_t key_pos = 0;
        size_t equal_pos = 0;
        // do
        // {
        //     and_pos = body_.find('&', key_pos);
        //     equal_pos = body_.find('=', key_pos);
        //     if(and_pos)
        //     /* code */
        // }
        while ((and_pos = body_.find('&', key_pos)) != body_.npos)
        {
            equal_pos = body_.find('=', key_pos);
            body_args_.insert({body_.substr(key_pos, equal_pos - key_pos),
                                body_.substr(equal_pos + 1, and_pos - equal_pos - 1)});
            
            key_pos = and_pos + 1;
        }
        equal_pos = body_.find('=', key_pos);
        body_args_.insert({body_.substr(key_pos, equal_pos - key_pos),
                    body_.substr(equal_pos + 1, body_.size() - equal_pos - 1)});
        
        return HttpContext::ParseBodysRet::PARSE_SUCCESS;

    }

}

HttpContext::AnalysissRet HttpContext::analysis_and_build()
{
    if(method_ == Method::GET)
    {
        //we need the url and the raw_file_name
        //1. /
        //2. /index.html
        std::string abs_filename = std::string("/home/zyf/webserver/wwwroot");

        abs_filename.append(url_raw_path_);
        if(abs_filename.back() == '/')
            url_raw_path_.append("index.html");
        
        struct stat buf;
        stat(abs_filename.data(), &buf);

        if(S_ISREG(buf.st_mode)  && (buf.st_mode & S_IRUSR))
        {
            FILE* file = fopen(abs_filename.c_str(), "r");
            size_t file_sz = buf.st_size;
            void *dynamicmem = malloc(file_sz);
            size_t nread = fread(dynamicmem, 1, file_sz, file);
            if(nread != file_sz)
                statuscode_ = StatusCode::ERROR;
            else 
            {
                statuscode_ == StatusCode::OK;
                outstr_.append("HTTP/1.0 200 OK\r\n\r\n");
                outstr_.append((char*)dynamicmem, file_sz);
            }

            fclose(file);
            //outstr_.append("HTTP/1.1 200 OK\r\n\r\n");
            free(dynamicmem);
            //return HttpContext::AnalysissRet::ANALYSIS_OK;
        }
        else
        {
            statuscode_ = StatusCode::FORBIDDEN;
            //return HttpContext::AnalysissRet::ANALYSIS_OK;
        }
        
    }
    else if(method_ == Method::POST)
    {
        //std::string abs_filename = std::string("/home/zyf/webserver/wwwroot");
        //abs_filename.append(url_raw_path_);
        
        //key1=value1&key2=value2
        //user=123&password=456&gender=male&age=12
        printf("body=%s\n", body_.data());
        if(url_raw_path_ != "/submit")
        {
            statuscode_ = StatusCode::ERROR;
            return AnalysissRet::ANALYSIS_OK;
        }

        
        std::string user = body_args_["name"];
        std::string passwd = body_args_["password"];
        std::string gender = body_args_["gender"];
        std::string age = body_args_["age"];
        printf("usr=%s\npasswd=%s\ngender=%s\nage=%s\n",user.data(), passwd.data(), gender.data(), age.data());

        MYSQL *con = mysql_init(NULL);
        
        if (con == NULL)
        {
            //fprin//not /submittf(stderr, "mysql_init() failed\n");
            statuscode_ = StatusCode::ERROR;
            return HttpContext::AnalysissRet::ANALYSIS_OK;
        }  
        
        if (mysql_real_connect(con, "localhost", "zyf", "zyf", "web", 0, NULL, 0) == NULL) 
        {
            fprintf(stderr, "mysql_real_connect() failed\n");
            mysql_close(con);

            statuscode_ = StatusCode::ERROR;
            return HttpContext::AnalysissRet::ANALYSIS_OK;
        }

        std::string sql = "select * from usr where name='";
        sql += user + "'";
        
        mysql_query(con, sql.data());

        MYSQL_RES *result = mysql_store_result(con);

        if (result == NULL) 
        {
            fprintf(stderr, "mysql_store_result() failed\n");
            mysql_close(con);

            statuscode_ = StatusCode::ERROR;
            return HttpContext::AnalysissRet::ANALYSIS_OK;
        }  

        MYSQL_ROW row;
        row = mysql_fetch_row(result);

        if(row == NULL) 
        {
            printf("no row\n");
            //insert
            std::string sql = "insert usr (name, password, gender, age) values (";
            sql += "'" + user + "',";
            sql += "'" + passwd + "',";
            sql += "'" + gender + "',";
            sql += "'" + age + "')" ;
            //cout << "sql:" << sql << endl;
            int ret = mysql_query(con, sql.data());

            if(ret == 0)
                statuscode_ = StatusCode::CREATED;
            else 
                statuscode_ = StatusCode::ERROR;

        }
        else 
        {
            printf("have row\n");
            std::string true_pw = row[2];
            printf("pwd=%s\n", true_pw.data());

            if(true_pw == passwd)
            {
                outstr_.append("HTTP/1.0 200 OK\r\n");
                outstr_.append("Content-Length: ");
                size_t length =  user.size() + gender.size() + age.size() + 115;
                outstr_.append(std::to_string(length));

                outstr_.append("\r\n\r\n<html><head><title>login/register success</title></head><body>");
                
                outstr_.append("your name:");
                outstr_.append(user);
                //outstr_.append("");
                outstr_.append("<br>your gender:");
                outstr_.append(gender);

                outstr_.append("<br>your age:");
                outstr_.append(age);

                outstr_.append("</body></html>");
                statuscode_ = StatusCode::OK;

                //return HttpContext::AnalysissRet::ANALYSIS_OK;

            }
            else 
            {
                statuscode_ =StatusCode::ERROR;
                //return HttpContext::AnalysissRet::ANALYSIS_OK;

            }

        }
        mysql_free_result(result);
        mysql_close(con);
        return HttpContext::AnalysissRet::ANALYSIS_OK;

        

    }
    return HttpContext::AnalysissRet::ANALYSIS_OK;

}
void HttpContext::build_response()
{
    if(statuscode_ == StatusCode::ERROR)
    {
        outstr_.append("HTTP/1.0 500 Internal Server Error\r\n");
        outstr_.append("Content-Length: 31\r\n");
        outstr_.append("\r\n<html><body>error</body></html>");
    }
    else if(statuscode_ == StatusCode::CREATED)
    {
        outstr_.append("HTTP/1.0 201 Created\r\n");
        outstr_.append("Content-Length: 40\r\n");
        outstr_.append("\r\n<html><body>insert success</body></html>");
    }
    else if(statuscode_ == StatusCode::OK) 
    {
        //do nothing because we have fill the response in analysis_and_build()
    }


}