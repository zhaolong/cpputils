#include "unitest.h"

#include <utils/http.h>
#include <utils/common.h>

BOOST_AUTO_TEST_SUITE(Http)


BOOST_AUTO_TEST_CASE(Synchronism)
{
    BOOST_CHECKPOINT("Synchronism");

    Utils::Http::Request request("http://www.baidu.com");
    BOOST_CHECK_EQUAL(request.Error(), Utils::Http::HTTP_NO_ERROR);
    BOOST_CHECK_EQUAL(Utils::Http::Instance().SendRequest(request), true);
    BOOST_CHECK_EQUAL(request.GetResponse()->StatusCode(), 200);
}

BOOST_AUTO_TEST_CASE(Asynchronous)
{
    BOOST_CHECKPOINT("Asynchronous");

    Utils::Http::Request request1("http://www.baidu.com");
    Utils::Http::Request request2("http://www.126.com");
    BOOST_CHECK_EQUAL(request1.Error(), Utils::Http::HTTP_NO_ERROR);
    BOOST_CHECK_EQUAL(request2.Error(), Utils::Http::HTTP_NO_ERROR);
    BOOST_CHECK_EQUAL(Utils::Http::Instance().AddRequest(request1), true);
    BOOST_CHECK_EQUAL(Utils::Http::Instance().AddRequest(request2), true);
    do 
    {
        Utils::Http::Instance().SendRequests();
        Utils::Sleep(500);
    } while (Utils::Http::Instance().Running());
    BOOST_CHECK_EQUAL(request1.GetResponse()->StatusCode(), 200);
    BOOST_CHECK_EQUAL(request2.GetResponse()->StatusCode(), 200);
}

BOOST_AUTO_TEST_CASE(RemoveRequest)
{
    BOOST_CHECKPOINT("RemoveRequest");

    Utils::Http::Request request("http://www.baidu.com");
    BOOST_CHECK_EQUAL(request.Error(), Utils::Http::HTTP_NO_ERROR);
    BOOST_CHECK_EQUAL(Utils::Http::Instance().AddRequest(request), true);
    do 
    {
        Utils::Http::Instance().SendRequests();
        Utils::Sleep(500);
    } while (Utils::Http::Instance().Running());
    BOOST_CHECK_EQUAL(request.GetResponse()->StatusCode(), 200);

    BOOST_CHECK_EQUAL(Utils::Http::Instance().RemoveRequest(request), true);
    BOOST_CHECK_EQUAL(request.SetUrl("http://www.126.com"), true);
    BOOST_CHECK_EQUAL(request.Error(), Utils::Http::HTTP_NO_ERROR);

    BOOST_CHECK_EQUAL(Utils::Http::Instance().SendRequest(request), true);
    BOOST_CHECK_EQUAL(request.GetResponse()->StatusCode(), 200);

    BOOST_CHECK_EQUAL(Utils::Http::Instance().AddRequest(request), true);
    do 
    {
        Utils::Http::Instance().SendRequests();
        Utils::Sleep(500);
    } while (Utils::Http::Instance().Running());
    BOOST_CHECK_EQUAL(request.GetResponse()->StatusCode(), 200);
}

BOOST_AUTO_TEST_CASE(IPV6)
{
    BOOST_CHECKPOINT("IPV6");
    Utils::Http::Request request("http://[fe80::b42d:d6ea:cf3e:a19a%2511]/");
    BOOST_CHECK_EQUAL(request.Error(), Utils::Http::HTTP_NO_ERROR);
    BOOST_CHECK_EQUAL(Utils::Http::Instance().SendRequest(request), true);
    BOOST_CHECK_EQUAL(request.GetResponse()->StatusCode(), 200);
}


BOOST_AUTO_TEST_CASE(FileUpload)
{
    /*
    BOOST_CHECKPOINT("FileUpload");
    Utils::Http::Request request("http://192.168.29.150/Discuz_X2.5_SC_UTF8/misc.php?mod=swfupload&action=swfupload&operation=upload&fid=2");
    request.SetCookie("xiqp_2132_saltkey=rKz7xUKR; xiqp_2132_lastvisit=1359333841; xiqp_2132_sid=tcZ1Rr; xiqp_2132_lastact=1359337528%09home.php%09spacecp; xiqp_2132_sendmail=1; xiqp_2132_ulastactivity=fe18BB37d4NoKPrNMe%2F6BLbch99xHyh9vTdm0exN2uLZGkM6SSZI; xiqp_2132_auth=c7f6DZo5Z0w3w1krBJ72%2B2aNssL%2Fgi3V59lKU%2FIID%2F3k%2BW52Tadj7RKFOunHQlpx6GAXHYivD55KL7uot4Ai; xiqp_2132_nofavfid=1; xiqp_2132_onlineusernum=1; xiqp_2132_noticeTitle=1; xiqp_2132_home_diymode=1; xiqp_2132_checkpm=1");

    Utils::Http::UploadFile uploadFile;
    uploadFile.name = "Filedata";
    uploadFile.file = "dbapp.jpg";
    uploadFile.formFields.push_back(Utils::Http::FormField("Filename", "dbapp.jpg"));
    uploadFile.formFields.push_back(Utils::Http::FormField("hash", "19d97e801e6dcde27f685f6015a12578"));
    uploadFile.formFields.push_back(Utils::Http::FormField("filetype", ".jpg"));
    uploadFile.formFields.push_back(Utils::Http::FormField("uid", "2"));
    uploadFile.formFields.push_back(Utils::Http::FormField("type", "image"));
    request.SetUploadFile(uploadFile);

    request.SetProxy("http://127.0.0.1:9090");
    BOOST_CHECK_EQUAL(Utils::Http::Instance().SendRequest(request), true);
    BOOST_CHECK_EQUAL(request.GetResponse()->StatusCode(), 200);
    */

    //Utils::Http::Request request("http://192.168.29.177//discuz%202.0/forum.php?mod=post&action=reply&fid=2&tid=1&extra=&replysubmit=yes");
    //request.SetCookie("tjpctrl=1359018175086; wjc2_2132_saltkey=bw5Bkgu9; wjc2_2132_lastvisit=1359012382; wjc2_2132_sid=zXhv1Y; wjc2_2132_lastact=1359016380%09forum.php%09ajax; wjc2_2132_sendmail=1; wjc2_2132_ulastactivity=1ef0UTIoi51IhfhcbToZYlne7BDa3Br76AiNDs4pmTipPi7lF9dZ; wjc2_2132_auth=654dY0LZzQJO9H8uhpk9RWebo6xmr%2BXaVEUJwwGCPmbSYErzAPWamnd%2FjU1V4ig4YeMNVmIjtiE%2FJmq2WE%2Ft; wjc2_2132_lastcheckfeed=3%7C1359016369; wjc2_2132_checkfollow=1; wjc2_2132_nofavfid=1; wjc2_2132_onlineusernum=1; wjc2_2132_checkpm=1; wjc2_2132_forum_lastvisit=D_2_1359016374; wjc2_2132_visitedfid=2; wjc2_2132_smile=1D1; wjc2_2132_editormode_e=1");
    //request.SetPost("formhash=0d6c5ab4&wysiwyg=1&noticeauthor=&noticetrimstr=&noticeauthormsg=&subject=&unused%5B%5D=23&message=%5Battachimg%5D23%5B%2Fattachimg%5D%0D%0Assss&save=&attachnew%5B23%5D%5Bdescription%5D=%3Cimg+src%3D%23+onerror%3Dalert%28%2Fdz_xss_dbapp%2F%29%3E&usesig=1");
    //// request.SetProxy("http://127.0.0.1:9090");
    //request.FollowLocation(5);
    //BOOST_CHECK_EQUAL(Utils::Http::Instance().SendRequest(request), true);
    //printf("%s", request.GetResponse()->Body());
}

BOOST_AUTO_TEST_SUITE_END() /* Http Test */