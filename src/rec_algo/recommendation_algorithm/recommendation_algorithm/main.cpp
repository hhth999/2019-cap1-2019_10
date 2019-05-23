#pragma comment(lib, "lib_json.lib")
#pragma warning(disable:4996)

#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <functional>
#include <memory>
#include "json/json.h"

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
	7 : ���� ���̺� (������ �ʿ����)
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

		//��ó���� ����ó���� �ϸ鼭 6���� �������� �ٲٰ�, �̸� emotion_res�� �����Ѵ�.
		map<double, string, greater<double> > emotion_res;

		//��ó��
		for (auto & it : tmp_res) {
			cout << it.first << " " << it.second << endl; //�����
			if ( (it.second == "HAPPINESS" && it.first >= 0.2 && tone == 0 ) /*�ູ�󱼷� ���¸�Ҹ�*/
				|| (it.second == "SAD" && it.first >= 0.2 && tone == 1) /*���¾󱼷� �ູ��Ҹ�*/ ) { 
				//Q. �ູ�� �󸶳� ���;� ������� ����� ó���ұ�
				table = 0;
				cout << "������ ���̺�� ������ �����մϴ�" << endl;
			}
		}
		return 0; //������ ����� ������ ������ �� -> 83�� ���� ������ �߰�

	}
	else cout << "parse failed" << endl; 
	return 0;
}