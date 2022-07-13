#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include "face.h"


using namespace std;
using namespace cv;


int main(){
	// 设置APPID/AK/SK
    string app_id = "26670390";
    string api_key = "fOHQ3BquKWU2EHe9UuVKGwve";
    string secret_key = "BOjUWMnhHfbmp1dezcLeGZgsaj3jUaxE";
	aip::Face client(app_id, api_key, secret_key);
	printf("Use opencv2 to cap photo!\n");
	VideoCapture cap(0);

	Mat photo;//comeplete a Mat object 
	Mat gray;
	Mat grayHist;
	Mat detectFace;
	//jilian classtfier
	CascadeClassifier calssifier("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt2.xml");
	vector<Rect> allFaceRetangle;
	vector<uchar> result;
	string Base64Face;
	Json::Value FResult;
	time_t current;
	//make a while photo
	while(cap.isOpened()){
		cap>>photo;
		cvtColor(photo,gray,CV_BGR2GRAY);
		//imshow("photo",photo);
		//adjust liangdu junhengdu
		equalizeHist(gray,grayHist);
		//detect allface and make the list of retancle
		calssifier.detectMultiScale(grayHist,allFaceRetangle);
		if(allFaceRetangle.size()){
			rectangle(grayHist,allFaceRetangle[0],Scalar(255,255,255));
			//get result jpg
			detectFace = grayHist(allFaceRetangle[0]);
			imencode(".jpg",detectFace,result);
			Base64Face = aip::base64_encode((char *)result.data(),result.size());
			/* **1：N人脸识别**与**1：N人脸认证**的差别在于：人脸搜索是在指定人脸集合中进行直接地人脸检索操作，而人脸认证是基于uid，先调取这个uid对应的人脸，再在这个uid对应的人脸集合中进行检索（因为每个uid通常对应的只有一张人脸，所以通常也就变为了1：1对比）；实际应用中，人脸认证需要用户或系统先输入id，这增加了验证安全度，但也增加了复杂度，具体使用哪个接口需要视您的业务场景判断。
         * @param image 图片信息(**总数据大小应小于10M**)，图片上传方式根据image_type来判断
         * @param image_type 图片类型 **BASE64**:图片的base64值，base64编码后的图片数据，需urlencode，编码后的图片大小不超过2M；**URL**:图片的 URL地址( 可能由于网络等原因导致下载图片时间过长)**；FACE_TOKEN**: 人脸图片的唯一标识，调用人脸检测接口时，会为每个人脸图片赋予一个唯一的FACE_TOKEN，同一张图片多次检测得到的FACE_TOKEN是同一个
         * @param group_id_list 从指定的group中进行查找 用逗号分隔，**上限20个**
         * options 可选参数:
         * max_face_num 最多处理人脸的数目<br>**默认值为1(仅检测图片中面积最大的那个人脸)** **最大值10**
         * match_threshold 匹配阈值（设置阈值后，score低于此阈值的用户信息将不会返回） 最大100 最小0 默认80 <br>**此阈值设置得越高，检索速度将会越快，推荐使用默认阈值`80`**
         * quality_control 图片质量控制  **NONE**: 不进行控制 **LOW**:较低的质量要求 **NORMAL**: 一般的质量要求 **HIGH**: 较高的质量要求 **默认 NONE**
         * liveness_control 活体检测控制  **NONE**: 不进行控制 **LOW**:较低的活体要求(高通过率 低攻击拒绝率) **NORMAL**: 一般的活体要求(平衡的攻击拒绝率, 通过率) **HIGH**: 较高的活体要求(高攻击拒绝率 低通过率) **默认NONE**
         * user_id 当需要对特定用户进行比对时，指定user_id进行比对。即人脸认证功能。
         * max_user_num 查找后返回的用户数量。返回相似度最高的几个用户，默认为1，最多返回50个。
         */
			FResult = client.search(Base64Face,"BASE64","alpha",aip::null);
			//cout<<FResult<<endl;
			//if 
			if(!FResult["result"].isNull()){
				if(FResult["result"]["user_list"][0]["score"].asInt() > 70){
					cout << FResult["result"]["user_list"][0]["user_id"] <<endl;
					current = time(NULL);
					cout << "The time is ";
					//cout << current << endl;
					cout << ctime(&current) <<endl;
					//由于将显示重定向到了后台，所以需要将识别成功的信息显示在图像上
					putText(grayHist, FResult["result"]["user_list"][0]["user_id"].asString(), Point(0,50), FONT_HERSHEY_PLAIN, 1, Scalar (0,0,255));
				}
				else{
					cout << "is false,because the scaore is";
					cout<<FResult["result"]["user_list"][0]["score"].asInt()<< endl;
					cout << "The time is ";
					cout << ctime(&current) <<endl;
				}
			}
			
		}
		imshow("gray",grayHist);
		if(waitKey(20)==27){
			//esc ascii ==27
			break;
		}
	}
	if(!cap.isOpened()){
		cout<<"something error!"<<endl;
		return -1;
	}
}
