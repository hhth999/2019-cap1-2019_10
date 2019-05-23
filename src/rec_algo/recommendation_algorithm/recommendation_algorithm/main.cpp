#pragma comment(lib, "lib_json.lib")
#pragma warning(disable:4996)

#include "json/json.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <map>
#include <ctime>
#include <functional>
#include <memory>

using namespace std;
using namespace Json;

int main() {

	ifstream json_dir("JSON_DATA.json");
	CharReaderBuilder builder;
	builder["collectComments"] = false;
	
	Value root; //Json::Value root;
	Reader reader; //Json::Reader reader;

	JSONCPP_STRING errs;
	bool ok = parseFromStream(builder, json_dir, &root, &errs);

	//multimap : �ߺ�Ű ��� & ���ĵ� 
	multimap<double, string, greater<double> > tmp_res; //greater<int>�� key�� ������������ ���ĵȴ�. 

	// ���� ��� - �ϴ� int������
	int tone = 0; 
	/* 0 : ������� ��� �� ���� ��Ҹ�(���� �ູ), 
	   1 : ������� ��� �� �ູ ��Ҹ�(���� ����)
	   2 : �߸��� 0.8�̻�
	   3 : ������
	*/

	//������ ���̺�
	int table = 0; 
	/*
	0 : ������ ���̺�
	1 : ���� ���̺��� HAPPINESS
	2 : ���� ���̺��� SAD
	3 : ���� ���̺��� CONTEMPT_DISGUST
	4 : ���� ���̺��� FEAR
	5 : ���� ���̺��� ANGER
	6 : ���� ���̺��� SURPRISE
	* �������̺��� ���ɸ� �ʿ��ϹǷ�, ���̽� �������� �� ���� ������ �˻��ϰ� ������ �����̸� ��õ�˰����� �ʿ����. 
	*/

	if (ok == true) {
		//json���Ͽ��� �� ������ �ش��ϴ� ������ ������ double���� ������ �ִ´�. (face API �����)
		double anger = root.get("ANGER", 0.0).asDouble();
		double fear = root.get("FEAR", 0.0).asDouble();
		double disgust = root.get("DISGUST", 0.0).asDouble();
		double contempt = root.get("CONTEMPT", 0.0).asDouble();
		double neutral = root.get("NEUTRAL", 0.0).asDouble();
		double happiness = root.get("HAPPINESS", 0.0).asDouble();
		double sad = root.get("SAD", 0.0).asDouble();
		double surprise = root.get("SURPRISE", 0.0).asDouble();

		//multimap�� tmp_res�� key-value ���·� �����Ѵ�. key : ����, value : ����
		tmp_res.insert(make_pair(anger, "ANGER"));
		tmp_res.insert(make_pair(fear, "FEAR"));
		tmp_res.insert(make_pair(contempt, "CONTEMPT"));
		tmp_res.insert(make_pair(disgust, "DISGUST"));
		tmp_res.insert(make_pair(happiness, "HAPPINESS"));
		tmp_res.insert(make_pair(neutral, "NEUTRAL"));
		tmp_res.insert(make_pair(sad, "SAD"));
		tmp_res.insert(make_pair(surprise,"SURPRISE"));

		//tmp_res ���� ����-���� pair�� ����ִ�. ( 0.6, 'FEAR' ��) 

		//��ó��(����ó��, ������)�� �ϸ鼭 6���� �������� �ٲٰ�, �̸� emotion_res�� �����Ѵ�.
		multimap<double, string, greater<double> > emotion_res;

		//��ó��
		double tmp_contempt_disgust = 0.0; //emotion_res�� ���� ���ο� ���� contempt_disgust �� ���� ������ ��
		for (auto & it : tmp_res) {
			//cout << it.first << " " << it.second << endl; //�����

			//��ó�� - 0.05�̸��� 0.0���� ������ + 6���� �������� ����� emotion_res�� �����Ѵ�. 
			if (it.second != "NEUTRAL") { //�߸��� �������̺� �����Ƿ� emotion_res�� ������ �ȵȴ�. 
				if (it.second == "CONTEMPT" || it.second == "DISGUST") {
					tmp_contempt_disgust += it.first; //for�� ������ emotion_res�� �ִ´�.  
				}
				else {
					if (it.first < 0.05)
						emotion_res.insert(make_pair(0.0, it.second));
					else
						emotion_res.insert(make_pair(it.first, it.second));
				}
			}

			//����ó�� (1) ������ �Ǻ� : ���İ� ����� ���� ���� - face�� ���� API ����� ������̸� ����ó��
			if ( (it.second == "HAPPINESS" && it.first >= 0.2 && tone == 0 ) /*�ູ�󱼷� ���¸�Ҹ�*/
				|| (it.second == "SAD" && it.first >= 0.2 && tone == 1) /*���¾󱼷� �ູ��Ҹ�*/ ) { 
				//Q. �ູ�� �󸶳� ���;� ������� ����� ó���ұ�
				table = 0;
				cout << "������ ���̺�� ������ �����մϴ�" << endl;
				//return 0;
			}
			
			//����ó�� (2) neutral �߸��� ũ�� ��������(���� 0.8)�� ���� �뷡 3�� ���� 
			else if (it.second == "NEUTRAL" && it.first >= 0.8) {
				srand((size_t)time(NULL)); 
				for (int num = 0; num < 3; num++) {
					cout << "���� ���̺��� : " << (rand() % 6) + 1 << endl; //���� 3���� ������ ����
				}
				//return 0;
			}
		}
		//for�� ���ͼ��� tmp_contempt_disgust�� emotion_res�� ����(���� ��ó��[������] ����)
		if (tmp_contempt_disgust< 0.05) 
			emotion_res.insert(make_pair(0.0, "CONTEMPT_DISGUST"));
		else 
			emotion_res.insert(make_pair(tmp_contempt_disgust, "CONTEMPT_DISGUST"));

		//�����
		cout << endl;
		for (auto &iter : emotion_res) {
			cout << iter.first << " " << iter.second << endl;
		}
	}
	else cout << "parse failed" << endl; 
	return 0;
}