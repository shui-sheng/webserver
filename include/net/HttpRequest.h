#ifndef _HTTPREQUEST_H_
#define _HTTPREQUEST_H_
#include"../log/Logger.h"
#include"Buffer.h"
#include<string>
#include<iostream>
#include<map>
#include<unordered_map>
#include<assert.h>

namespace fei
{
class HttpRequest
{
public:
    enum class Method { GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH };
    enum class Version { HTTP10, HTTP11, HTTP2, HTTP3 };

public:
    
    
    void loadRequest(Buffer& buffer);


    // GET / HTTP/1.1\r\n
    void  loadRequestLine(std::string& rawRequest);
    // GET / HTTP/1.1
    // Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
    // Accept-Encoding: gzip, deflate, br
    void loadHeaders(std::string& rawRequest);
    void loadMessageBody(std::string& rawRequest);

    Method getMethod() { return method_; }
    Version getVersion() { return version_; }
    std::string getTarget() { return target_; }


    //return the idx of '\r'
    size_t findCRLF(std::string& str, size_t pos)
    {
        size_t CRidx = 0;
        //  \r\n
            // HTTP/1.1 200 OK\r\n
            // Date: Sun, 21 Jan 2024 08:30:01 GMT\r\n
        if((CRidx = str.find('\r', pos)) != str.npos)
        {
            if(str[CRidx + 1] == '\n')
            {
                return CRidx;
            }
        }

        return str.npos;
    }
    size_t SearchChar(std::string& str, char ch, size_t pos, size_t count)
    {
        while(count > 0)
        {
            if(str[pos] == ch)
                return pos;
            pos++;
        }
        return std::string::npos;
    }
    Method method_;
    std::string target_;
    Version version_;
    std::unordered_map<std::string, std::string> headers_;
    std::string message_body_;

    size_t nextPhraseIdx_ = 0;
    char* readIdx_ = NULL;




};

}


// GET / HTTP/1.1
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
// Accept-Encoding: gzip, deflate, br







// GET / HTTP/1.1
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
// Accept-Encoding: gzip, deflate, br
// Accept-Language: zh-CN,zh;q=0.9,en;q=0.8
// Cache-Control: max-age=0
// Connection: keep-alive
// Cookie: uuid_tt_dd=10_19729136210-1688480461464-201018; historyList-new=%5B%5D; Hm_lvt_e5ef47b9f471504959267fd614d579cd=1695473733; UN=flyeahfeifei; p_uid=U010000; ssxmod_itna2=WqjOY5GIlQ0LeYKHmCDyG85DtDOtDG2xQ4A=uAwqD/UBbDFx7IS74NqFGFB+Z+=G5hs2U0qYi4lIPdR=zNqx1SnpTsRGac760bkLTf+HULOuRRWRMG27CB=A74zH441HeGRtUwY8zlxiVA2AA+O3SQwj00bPmieNE+r2oaeAESkRR9liLlWRYcQbtoPWhRdbNAkRYKY+AIrKuUAEYBGq7u43GmkEvjkEImMYOYIPwq4gMebK/FK0IFMQA8STLhfTjhuRKu9nFhpdSI3ahD8vZ4dcL+o1qdXHFz3cw+sQ9vynRsSD11Vo4gzCqG3wssds5lHPj1omhSda0gzmMh5lY1+d6k3qYezuzPf1O2khCQLgdp4dcGo++5u87LCrpS1wS1AmK6YaNxeG+DIDDwghOChs6rdiqMenhtXD8Wh9P+zqtY5h8veBhGUnAtcd75C=NLe4sjfsBxEIa4eF8DePDGcDG7+Lqa7Er0zF1zCAwhB+77duGqu7z17d50quwDxterYe4E4mq6dcq4jUzgaHEUmkDDI09uL3txuiDD==; ins_first_time=1698291220281; UserName=flyeahfeifei; UserInfo=7d6603a9d4134a93a7554bae17564f04; UserToken=7d6603a9d4134a93a7554bae17564f04; UserNick=%E6%9C%89%E7%82%B9Giao%E7%AC%91; AU=B65; BT=1698395408326; Hm_up_6bcd52f51e9b3dce32bec4a3997715ac=%7B%22islogin%22%3A%7B%22value%22%3A%221%22%2C%22scope%22%3A1%7D%2C%22isonline%22%3A%7B%22value%22%3A%221%22%2C%22scope%22%3A1%7D%2C%22isvip%22%3A%7B%22value%22%3A%220%22%2C%22scope%22%3A1%7D%2C%22uid_%22%3A%7B%22value%22%3A%22flyeahfeifei%22%2C%22scope%22%3A1%7D%7D; FCNEC=%5B%5B%22AKsRol_sx11ErYkJgnIzGJHreAdwLvaH2xGi72pg7pkXomT3IuipuoJDln0cYy4M-K6GdmvoC761XcobPChfHw7iesZ53CkZLe6OpXg3C8hQclNrp3ckPiq00ZpKFR0VxQvZGkOdSs9ajFo8hc1yXabEL2aCFa86Fg%3D%3D%22%5D%2Cnull%2C%5B%5D%5D; __gads=ID=6e58202033d59f0d-2290198acde30097:T=1694402202:RT=1699258079:S=ALNI_MbndBpJ5uS-8Ta-dYWT9_fC4HmAfw; __gpi=UID=00000c40bea62910:T=1694402202:RT=1699258079:S=ALNI_MbfKbnApYaYU0-onjVmfAQwPgyxJQ; fpv=277a2bd03d789c3ae16ee2b601c15042; ssxmod_itna=eqUx0QitGQi=n47q0dD=wO2jf=TLx7IAPDCRRDBkAO4iNDnD8x7YDvImL3=IF9lW+i3NhR0003qlFrATfbtbpj5KDQxY=DUxAtDBxDeXp+DBLYaRixii9DCmaDPxKD3LxneD+D047lgxqDE+uKDz4Gm4jX4xDtD3TdBDL4DCo4DboPyDB6=xBQD7ur0tWdDKwRS/MbsfYPtIAvz87piKAmef0GrbdehF0hCN29Y+Yx4R1GP+pOOPD===; c_dl_prid=1703036831678_529431; c_dl_rid=1705565269506_454929; c_dl_fref=https://blog.csdn.net/m0_67254672/article/details/129257179; c_dl_fpage=/download/weixin_38689477/12743115; c_dl_um=distribute.pc_search_result.none-task-blog-2%7Eall%7Esobaiduend%7Edefault-1-129257179-null-null.142%5Ev99%5Epc_search_result_base7; _ga_7W1N0GEY1P=GS1.1.1705738095.8.0.1705738095.60.0.0; _ga=GA1.2.2092038369.1696997057; https_waf_cookie=9309ca25-e93f-4bb186e3f81e5b32051dc0b1b5e64a152c44; dc_session_id=10_1705823290532.485342; csrfToken=IXLRcCKWqfkd1HJ_q762US5X; c_segment=10; www_red_day_last=red; Hm_lvt_6bcd52f51e9b3dce32bec4a3997715ac=1705407004,1705548999,1705737937,1705823303; SidecHatdocDescBoxNum=true; c_first_ref=default; c_first_page=https%3A//www.csdn.net/; c_dsid=11_1705824348062.085783; fe_request_id=1705824375782_7431_3307161; log_Id_click=1316; c_utm_relevant_index=1; relevant_index=1; c_pref=https%3A//www.csdn.net/; c_ref=https%3A//so.csdn.net/so/search%3Fspm%3D1000.2115.3001.4498%26q%3Dhttpversion%26t%3D%26u%3D; c_utm_medium=distribute.pc_search_result.none-task-blog-2%7Eall%7Esobaiduend%7Edefault-1-123870383-null-null.142%5Ev99%5Epc_search_result_base7; c_utm_term=httpversion; c_page_id=default; log_Id_pv=630; log_Id_view=6418; Hm_lpvt_6bcd52f51e9b3dce32bec4a3997715ac=1705824381; firstDie=1; referrer_search=1705824382028; dc_sid=de5dba24c041eb5277419b5c165f9cef; dc_tos=s7lrlw
// Host: www.csdn.net
// If-Modified-Since: Sun, 21 Jan 2024 08:05:01 GMT
// If-None-Match: W/"65acd02d-7ee32"
// Sec-Fetch-Dest: document
// Sec-Fetch-Mode: navigate
// Sec-Fetch-Site: none
// Sec-Fetch-User: ?1
// Upgrade-Insecure-Requests: 1
// User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36
// sec-ch-ua: "Not_A Brand";v="8", "Chromium";v="120", "Google Chrome";v="120"
// sec-ch-ua-mobile: ?0
// sec-ch-ua-platform: "Linux"




#endif