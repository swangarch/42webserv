#include "../../includes/infopage.class.hpp"

std::map<std::string, std::string>    infopage::initResType()
{
    std::map<std::string, std::string>   typeTable;

    typeTable[".*"] = "application/octet-stream";
    typeTable[".001"] = "application/x-001";
    typeTable[".323"] = "text/h323";
    typeTable[".906"] = "application/x-906";
    typeTable[".907"] = "drawing/907";
    typeTable[".a11"] = "application/x-a11";
    typeTable[".acp"] = "audio/x-mei-aac";
    typeTable[".ai"] = "application/postscript";
    typeTable[".aif"] = "audio/aiff";
    typeTable[".aifc"] = "audio/aiff";
    typeTable[".aiff"] = "audio/aiff";
    typeTable[".anv"] = "application/x-anv";
    typeTable[".asa"] = "text/asa";
    typeTable[".asf"] = "video/x-ms-asf";
    typeTable[".asp"] = "text/asp";
    typeTable[".asx"] = "video/x-ms-asf";
    typeTable[".au"] = "audio/basic";
    typeTable[".avi"] = "video/avi";
    typeTable[".awf"] = "application/vnd.adobe.workflow";
    typeTable[".biz"] = "text/xml";
    typeTable[".bmp"] = "application/x-bmp";
    typeTable[".bot"] = "application/x-bot";
    typeTable[".c4t"] = "application/x-c4t";
    typeTable[".c90"] = "application/x-c90";
    typeTable[".cal"] = "application/x-cals";
    typeTable[".cat"] = "application/vnd.ms-pki.seccat";
    typeTable[".cdf"] = "application/x-netcdf";
    typeTable[".cdr"] = "application/x-cdr";
    typeTable[".cel"] = "application/x-cel";
    typeTable[".cer"] = "application/x-x509-ca-cert";
    typeTable[".cg4"] = "application/x-g4";
    typeTable[".cgm"] = "application/x-cgm";
    typeTable[".cit"] = "application/x-cit";
    typeTable[".class"] = "java/*";
    typeTable[".cml"] = "text/xml";
    typeTable[".cmp"] = "application/x-cmp";
    typeTable[".cmx"] = "application/x-cmx";
    typeTable[".cot"] = "application/x-cot";
    typeTable[".crl"] = "application/pkix-crl";
    typeTable[".crt"] = "application/x-x509-ca-cert";
    typeTable[".csi"] = "application/x-csi";
    typeTable[".css"] = "text/css";
    typeTable[".cut"] = "application/x-cut";
    typeTable[".dbf"] = "application/x-dbf";
    typeTable[".dbm"] = "application/x-dbm";
    typeTable[".dbx"] = "application/x-dbx";
    typeTable[".dcd"] = "text/xml";
    typeTable[".dcx"] = "application/x-dcx";
    typeTable[".der"] = "application/x-x509-ca-cert";
    typeTable[".dgn"] = "application/x-dgn";
    typeTable[".dib"] = "application/x-dib";
    typeTable[".dll"] = "application/x-msdownload";
    typeTable[".doc"] = "application/msword";
    typeTable[".dot"] = "application/msword";
    typeTable[".drw"] = "application/x-drw";
    typeTable[".dtd"] = "text/xml";
    typeTable[".dwf"] = "Model/vnd.dwf";
    typeTable[".dwf"] = "application/x-dwf";
    typeTable[".dwg"] = "application/x-dwg";
    typeTable[".dxb"] = "application/x-dxb";
    typeTable[".dxf"] = "application/x-dxf";
    typeTable[".edn"] = "application/vnd.adobe.edn";
    typeTable[".emf"] = "application/x-emf";
    typeTable[".eml"] = "message/rfc822";
    typeTable[".ent"] = "text/xml";
    typeTable[".epi"] = "application/x-epi";
    typeTable[".eps"] = "application/x-ps";
    typeTable[".eps"] = "application/postscript";
    typeTable[".etd"] = "application/x-ebx";
    typeTable[".exe"] = "application/x-msdownload";
    typeTable[".fax"] = "image/fax";
    typeTable[".fdf"] = "application/vnd.fdf";
    typeTable[".fif"] = "application/fractals";
    typeTable[".fo"] = "text/xml";
    typeTable[".frm"] = "application/x-frm";
    typeTable[".g4"] = "application/x-g4";
    typeTable[".gbr"] = "application/x-gbr";
    typeTable[".gif"] = "image/gif";
    typeTable[".gl2"] = "application/x-gl2";
    typeTable[".gp4"] = "application/x-gp4";
    typeTable[".hgl"] = "application/x-hgl";
    typeTable[".hmr"] = "application/x-hmr";
    typeTable[".hpg"] = "application/x-hpgl";
    typeTable[".hpl"] = "application/x-hpl";
    typeTable[".hqx"] = "application/mac-binhex40";
    typeTable[".hrf"] = "application/x-hrf";
    typeTable[".hta"] = "application/hta";
    typeTable[".htc"] = "text/x-component";
    typeTable[".htm"] = "text/html";
    typeTable[".html"] = "text/html";
    typeTable[".htt"] = "text/webviewhtml";
    typeTable[".htx"] = "text/html";
    typeTable[".icb"] = "application/x-icb";
    typeTable[".ico"] = "image/x-icon";
    typeTable[".ico"] = "application/x-ico";
    typeTable[".iff"] = "application/x-iff";
    typeTable[".ig4"] = "application/x-g4";
    typeTable[".igs"] = "application/x-igs";
    typeTable[".iii"] = "application/x-iphone";
    typeTable[".img"] = "application/x-img";
    typeTable[".ins"] = "application/x-internet-signup";
    typeTable[".isp"] = "application/x-internet-signup";
    typeTable[".IVF"] = "video/x-ivf";
    typeTable[".java"] = "java/*";
    typeTable[".jfif"] = "image/jpeg";
    typeTable[".jpe"] = "image/jpeg";
    typeTable[".jpeg"] = "image/jpeg";
    typeTable[".jpg"] = "image/jpeg";
    typeTable[".js"] = "application/x-javascript";
    typeTable[".jsp"] = "text/html";
    typeTable[".la1"] = "audio/x-liquid-file";
    typeTable[".lar"] = "application/x-laplayer-reg";
    typeTable[".latex"] = "application/x-latex";
    typeTable[".lars"] = "audio/x-liquid-secure";
    typeTable[".lbm"] = "application/x-lbm";
    typeTable[".lmsff"] = "audio/x-la-lms";
    typeTable[".ls"] = "application/x-javascript";
    typeTable[".ltr"] = "application/x-ltr";
    typeTable[".m1v"] = "video/x-mpeg";
    typeTable[".m2v"] = "video/x-mpeg";
    typeTable[".m3u"] = "audio/mpegurl";
    typeTable[".m4e"] = "video/mpeg4";
    typeTable[".mac"] = "application/x-mac";
    typeTable[".man"] = "application/x-troff-man";
    typeTable[".math"] = "text/xml";
    typeTable[".mdb"] = "application/msaccess";
    typeTable[".mdb"] = "application/x-mdb";
    typeTable[".mfp"] = "application/x-shockwave-flash";
    typeTable[".mht"] = "message/rfc822";
    typeTable[".mhtml"] = "message/rfc822";
    typeTable[".mi"] = "application/x-mi";
    typeTable[".mid"] = "audio/mid";
    typeTable[".midi"] = "audio/mid";
    typeTable[".mil"] = "application/x-mil";
    typeTable[".mml"] = "text/xml";
    typeTable[".mnd"] = "audio/x-musicnet-download";
    typeTable[".mns"] = "audio/x-musicnet-stream";
    typeTable[".mocha"] = "application/x-javascript";
    typeTable[".movie"] = "video/x-sgi-movie";
    typeTable[".mp1"] = "audio/mp1";
    typeTable[".mp2"] = "audio/mp2";
    typeTable[".mp2v"] = "video/mpeg";
    typeTable[".mp3"] = "audio/mp3";
    typeTable[".mp4"] = "video/mpeg4";
    typeTable[".mpa"] = "video/x-mpg";
    typeTable[".mpd"] = "application/vnd.ms-project";
    typeTable[".mpe"] = "video/x-mpeg";
    typeTable[".mpeg"] = "video/mpg";
    typeTable[".mpg"] = "video/mpg";
    typeTable[".mpga"] = "audio/rn-mpeg";
    typeTable[".mpp"] = "application/vnd.ms-project";
    typeTable[".mps"] = "video/x-mpeg";
    typeTable[".mpt"] = "application/vnd.ms-project";
    typeTable[".mpv"] = "video/mpg";
    typeTable[".mpv2"] = "video/mpeg";
    typeTable[".mpw"] = "application/vnd.ms-project";
    typeTable[".mpx"] = "application/vnd.ms-project";
    typeTable[".mtx"] = "text/xml";
    typeTable[".mxp"] = "application/x-mmxp";
    typeTable[".net"] = "image/pnetvue";
    typeTable[".nrf"] = "application/x-nrf";
    typeTable[".nws"] = "message/rfc822";
    typeTable[".odc"] = "text/x-ms-odc";
    typeTable[".out"] = "application/x-out";
    typeTable[".p10"] = "application/pkcs10";
    typeTable[".p12"] = "application/x-pkcs12";
    typeTable[".p7b"] = "application/x-pkcs7-certificates";
    typeTable[".p7c"] = "application/pkcs7-mime";
    typeTable[".p7m"] = "application/pkcs7-mime";
    typeTable[".p7r"] = "application/x-pkcs7-certreqresp";
    typeTable[".p7s"] = "application/pkcs7-signature";
    typeTable[".pc5"] = "application/x-pc5";
    typeTable[".pci"] = "application/x-pci";
    typeTable[".pcl"] = "application/x-pcl";
    typeTable[".pcx"] = "application/x-pcx";
    typeTable[".pdf"] = "application/pdf";
    typeTable[".pdf"] = "application/pdf";
    typeTable[".pdx"] = "application/vnd.adobe.pdx";
    typeTable[".pfx"] = "application/x-pkcs12";
    typeTable[".pgl"] = "application/x-pgl";
    typeTable[".pic"] = "application/x-pic";
    typeTable[".pko"] = "application/vnd.ms-pki.pko";
    typeTable[".pl"] = "application/x-perl";
    typeTable[".plg"] = "text/html";
    typeTable[".pls"] = "audio/scpls";
    typeTable[".plt"] = "application/x-plt";
    typeTable[".png"] = "image/png";
    typeTable[".pot"] = "application/vnd.ms-powerpoint";
    typeTable[".ppa"] = "application/vnd.ms-powerpoint";
    typeTable[".ppm"] = "application/x-ppm";
    typeTable[".pps"] = "application/vnd.ms-powerpoint";
    typeTable[".ppt"] = "application/vnd.ms-powerpoint";
    typeTable[".ppt"] = "application/x-ppt";
    typeTable[".pr"] = "application/x-pr";
    typeTable[".prf"] = "application/pics-rules";
    typeTable[".prn"] = "application/x-prn";
    typeTable[".prt"] = "application/x-prt";
    typeTable[".ps"] = "application/postscript";
    typeTable[".ps"] = "application/x-ps";
    typeTable[".ptn"] = "application/x-ptn";
    typeTable[".pub"] = "application/x-ms-publisher";
    typeTable[".qt"] = "video/quicktime";
    typeTable[".qtc"] = "video/x-quicktime";
    typeTable[".ra"] = "audio/x-realaudio";
    typeTable[".ram"] = "audio/x-realaudio";
    typeTable[".ras"] = "application/x-ras";
    typeTable[".rdf"] = "text/xml";
    typeTable[".rdp"] = "application/x-rdp";
    typeTable[".ref"] = "text/xml";
    typeTable[".rgb"] = "application/x-rgb";
    typeTable[".rm"] = "audio/x-pn-realaudio";
    typeTable[".rmi"] = "audio/mid";
    typeTable[".rmj"] = "application/vnd.rn-realmedia-javascript";
    typeTable[".rmvb"] = "audio/x-pn-realaudio";
    typeTable[".rps"] = "application/x-rps";
    typeTable[".rtf"] = "application/rtf";
    typeTable[".rtf"] = "text/rtf";
    typeTable[".rtf"] = "text/plain";
    typeTable[".rtx"] = "text/richtext";
    typeTable[".scd"] = "application/x-scd";
    typeTable[".sct"] = "application/x-secure-file";
    typeTable[".sdw"] = "application/x-sdw";
    typeTable[".sem"] = "application/x-sem";
    typeTable[".setpay"] = "application/x-setpay";
    typeTable[".setreg"] = "application/x-setreg";
    typeTable[".sgml"] = "text/sgml";
    typeTable[".sh"] = "application/x-sh";
    typeTable[".shar"] = "application/x-shar";
    typeTable[".sit"] = "application/x-stuffit";
    typeTable[".sldm"] = "application/vnd.ms-powerpoint.slide.macroenabled.12";
    typeTable[".sldx"] = "application/vnd.openxmlformats-officedocument.presentationml.slide";
    typeTable[".smf"] = "application/x-smf";
    typeTable[".smv"] = "application/x-smv";
    typeTable[".snd"] = "audio/basic";
    typeTable[".sol"] = "application/sol";
    typeTable[".spx"] = "audio/x-speex";
    typeTable[".src"] = "application/x-wais-source";
    typeTable[".stl"] = "application/sla";
    typeTable[".stm"] = "audio/x-stm";
    typeTable[".sv4cpio"] = "application/x-sv4cpio";
    typeTable[".sv4crc"] = "application/x-sv4crc";
    typeTable[".swf"] = "application/x-shockwave-flash";
    typeTable[".t3"] = "application/x-t3";
    typeTable[".tar"] = "application/x-tar";
    typeTable[".taz"] = "application/x-tar";
    typeTable[".tbz"] = "application/x-tar";
    typeTable[".tcl"] = "application/x-tcl";
    typeTable[".tex"] = "application/x-tex";
    typeTable[".texi"] = "application/x-texinfo";
    typeTable[".texinfo"] = "application/x-texinfo";
    typeTable[".tgz"] = "application/x-tar";
    typeTable[".tiff"] = "image/tiff";
    typeTable[".tif"] = "image/tiff";
    typeTable[".tr"] = "application/x-troff";
    typeTable[".tsi"] = "audio/x-mpeg";
    typeTable[".tsv"] = "text/tab-separated-values";
    typeTable[".txt"] = "text/plain";
    typeTable[".uls"] = "text/iuls";
    typeTable[".ustar"] = "application/x-ustar";
    typeTable[".vcd"] = "application/x-cdlink";
    typeTable[".vcf"] = "text/x-vcard";
    typeTable[".vcs"] = "text/x-vcalendar";
    typeTable[".vda"] = "application/x-vda";
    typeTable[".vdx"] = "application/vnd.ms-visio.viewer";
    typeTable[".vml"] = "text/xml";
    typeTable[".vpg"] = "application/x-vpg";
    typeTable[".vsd"] = "application/vnd.visio";
    typeTable[".vss"] = "application/vnd.visio";
    typeTable[".vst"] = "application/vnd.visio";
    typeTable[".vsto"] = "application/x-vsto";
    typeTable[".vsx"] = "application/vnd.visio";
    typeTable[".vtx"] = "application/vnd.visio";
    typeTable[".wav"] = "audio/wav";
    typeTable[".wax"] = "audio/x-ms-wax";
    typeTable[".wbmp"] = "image/vnd.wap.wbmp";
    typeTable[".webm"] = "video/webm";
    typeTable[".wma"] = "audio/x-ms-wma";
    typeTable[".wmf"] = "image/wmf";
    typeTable[".wmv"] = "video/x-ms-wmv";
    typeTable[".wpd"] = "application/x-wpd";
    typeTable[".wri"] = "application/mswrite";
    typeTable[".wsdl"] = "text/xml";
    typeTable[".wspolicy"] = "text/xml";
    typeTable[".x12"] = "application/x-x12";
    typeTable[".x3d"] = "application/x-x3d+xml";
    typeTable[".xaml"] = "application/xaml+xml";
    typeTable[".xap"] = "application/x-silverlight-app";
    typeTable[".xar"] = "application/vnd.xara";
    typeTable[".xbm"] = "image/x-xbitmap";
    typeTable[".xcf"] = "image/x-xcf";
    typeTable[".xhtml"] = "application/xhtml+xml";
    typeTable[".xls"] = "application/vnd.ms-excel";
    typeTable[".xlsm"] = "application/vnd.ms-excel.sheet.macroenabled.12";
    typeTable[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    typeTable[".xml"] = "text/xml";
    typeTable[".xpl"] = "application/x-xpl";
    typeTable[".xsd"] = "text/xml";
    typeTable[".xsl"] = "text/xml";
    typeTable[".xslt"] = "text/xml";
    typeTable[".xwd"] = "image/x-xwd";
    typeTable[".z"] = "application/x-compress";
    typeTable[".zip"] = "application/zip";
    typeTable[".ttf"] = "font/ttf";

    return (typeTable);
}

std::map<int, std::string>    infopage::initStatusInfo()
{
    std::map<int, std::string> statusMap;
    
    statusMap[100] = "Continue";
    statusMap[101] = "Switching Protocols";
    statusMap[102] = "Processing";
    statusMap[103] = "Early Hints";
    
    statusMap[200] = "OK";
    statusMap[201] = "Created";
    statusMap[202] = "Accepted";
    statusMap[203] = "Non-Authoritative Information";
    statusMap[204] = "No Content";
    statusMap[205] = "Reset Content";
    statusMap[206] = "Partial Content";
    statusMap[207] = "Multi-Status";
    statusMap[208] = "Already Reported";
    statusMap[226] = "IM Used";
    
    statusMap[300] = "Multiple Choices";
    statusMap[301] = "Moved Permanently";
    statusMap[302] = "Found";
    statusMap[303] = "See Other";
    statusMap[304] = "Not Modified";
    statusMap[305] = "Use Proxy";
    statusMap[306] = "unused";
    statusMap[307] = "Temporary Redirect";
    statusMap[308] = "Permanent Redirect";
    
    statusMap[400] = "Bad Request";
    statusMap[401] = "Unauthorized";
    statusMap[402] = "Payment Required";
    statusMap[403] = "Forbidden";
    statusMap[404] = "Not Found";
    statusMap[405] = "Method Not Allowed";
    statusMap[406] = "Not Acceptable";
    statusMap[407] = "Proxy Authentication Required";
    statusMap[408] = "Request Timeout";
    statusMap[409] = "Conflict";
    statusMap[410] = "Gone";
    statusMap[411] = "Length Required";
    statusMap[412] = "Precondition Failed";
    statusMap[413] = "Payload Too Large";
    statusMap[414] = "URI Too Long";
    statusMap[415] = "Unsupported Media Type";
    statusMap[416] = "Range Not Satisfiable";
    statusMap[417] = "Expectation Failed";
    statusMap[418] = "I'm a teapot";
    statusMap[421] = "Misdirected Request";
    statusMap[422] = "Unprocessable Entity";
    statusMap[423] = "Locked";
    statusMap[424] = "Failed Dependency";
    statusMap[425] = "Too Early";
    statusMap[426] = "Upgrade Required";
    statusMap[428] = "Precondition Required";
    statusMap[429] = "Too Many Requests";
    statusMap[431] = "Request Header Fields Too Large";
    statusMap[451] = "Unavailable For Legal Reasons";
    
    statusMap[500] = "Internal Server Error";
    statusMap[501] = "Not Implemented";
    statusMap[502] = "Bad Gateway";
    statusMap[503] = "Service Unavailable";
    statusMap[504] = "Gateway Timeout";
    statusMap[505] = "HTTP Version Not Supported";
    statusMap[506] = "Variant Also Negotiates";
    statusMap[507] = "Insufficient Storage";
    statusMap[508] = "Loop Detected";
    statusMap[510] = "Not Extended";
    statusMap[511] = "Network Authentication Required";
    
    return statusMap;
}

std::string         infopage::getRessourceType(const std::string& type, std::map<std::string, std::string>& resType)
{
    return (resType[type]);
}

std::string         infopage::getStatusMsg(int status, std::map<int, std::string>  &statusInfo)
{
    return (statusInfo[status]);
}

std::string         infopage::getDefaultErrorPage(int status, std::map<int, std::string> &statusInfo)
{
    std::string defaultErrorPageP1 = "<!DOCTYPE html>\n"
                                   "<html lang=\"en\">\n"
                                   "<head>\n"
                                   "    <meta charset=\"UTF-8\">\n"
                                   "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                                   "    <title>";
                                   
    std::string defaultErrorPageP2 = "</title>\n"
                                   "    <style>\n"
                                   "        body {\n"
                                   "            font-family: Arial, sans-serif;\n"
                                   "            background-color: #f4f4f4;\n"
                                   "            text-align: center;\n"
                                   "            padding: 50px;\n"
                                   "            color: #333;\n"
                                   "        }\n"
                                   "        h1 {\n"
                                   "            font-size: 30px;\n"
                                   "            color: #cd763c;\n"
                                   "        }\n"
                                   "        p {\n"
                                   "            font-size: 24px;\n"
                                   "            margin-top: 15px;\n"
                                   "        }\n"
                                   "        a {\n"
                                   "            text-decoration: none;"
                                   "            color: #607575;\n"
                                   "            font-weight: bold;\n"
                                   "        }\n"
                                   "        a:hover {\n"
                                   "            text-decoration: underline;\n"
                                   "        }\n"
                                   "    </style>\n"
                                   "</head>\n"
                                   "<body>\n"
                                   "    <h1>webserv</h1>\n"
                                   "    <h1>";
                                   
    std::string defaultErrorPageP3 = "</h1>\n"
                                   "    <p>";

    std::string defaultErrorPageP4 = "</p>\n"
                                   "    <p><a href=\"/\">Go back to the homepage</a></p>\n"
                                   "</body>\n"
                                   "</html>\n";

    std::string info;
    if (status == 404)
        info = "Oops! The page you're looking for cannot be found.";
    else
        info = "Oops! There is something wrong.";
    std::ostringstream oss;

    oss << status << " " << getStatusMsg(status, statusInfo);

    std::string defaultErrorPage = defaultErrorPageP1 + oss.str() + defaultErrorPageP2 + oss.str() + defaultErrorPageP3 + info + defaultErrorPageP4;
    return (defaultErrorPage);
}

int         infopage::getInfoPage(std::string& body, const std::string& info)
{
    std::string defaultErrorPageP1 = "<!DOCTYPE html>\n"
                                   "<html lang=\"en\">\n"
                                   "<head>\n"
                                   "    <meta charset=\"UTF-8\">\n"
                                   "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                                   "    <title>";
                                   
    std::string defaultErrorPageP2 = "</title>\n"
                                   "    <style>\n"
                                   "        body {\n"
                                   "            font-family: Arial, sans-serif;\n"
                                   "            background-color: #f4f4f4;\n"
                                   "            text-align: center;\n"
                                   "            padding: 50px;\n"
                                   "            color: #333;\n"
                                   "        }\n"
                                   "        h1 {\n"
                                   "            font-size: 30px;\n"
                                   "            color: #cd763c;\n"
                                   "        }\n"
                                   "        p {\n"
                                   "            font-size: 24px;\n"
                                   "            margin-top: 15px;\n"
                                   "        }\n"
                                   "        a {\n"
                                   "            text-decoration: none;"
                                   "            color: #607575;\n"
                                   "            font-weight: bold;\n"
                                   "        }\n"
                                   "        a:hover {\n"
                                   "            text-decoration: underline;\n"
                                   "        }\n"
                                   "    </style>\n"
                                   "</head>\n"
                                   "<body>\n"
                                   "    <h1>webserv</h1>\n"
                                   "    <h1>"
                                   "</h1>\n"
                                   "    <p>";
                                   
    std::string defaultErrorPageP3 = "</h1>\n"
                                   "    <p>";

    std::string defaultErrorPageP4 = "</p>\n"
                                   "    <p><a href=\"/\">Go back to the homepage</a></p>\n"
                                   "</body>\n"
                                   "</html>\n";

    body = defaultErrorPageP1 + "webserv" + defaultErrorPageP2 + info + defaultErrorPageP3 + defaultErrorPageP4;
    return (1);
}