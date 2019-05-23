#pragma comment(lib, "lib_json.lib")
#pragma warning(disable:4996)

#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <functional> 
#include <typeinfo>
#include <memory>
#include "json/json.h"

using namespace std;
using namespace Json;

//enum Emotion {
//	ANGER,
//	FEAR,
//	HAPPINESS,
//	CONTEMP_dISGUST,
//	SAD,
//	SURPRISE
//};


int main() {
	ifstream json_dir("JSON_DATA.json");
	CharReaderBuilder builder;
	builder["collectComments"] = false;
	
	Value root; //Json::Value root;
	Reader reader; //Json::Reader reader;

	JSONCPP_STRING errs;
	bool ok = parseFromStream(builder, json_dir, &root, &errs);

	//multimap : �ߺ�Ű ��� / ���ĵ� 
	multimap<double, string, greater<double>> tmp_res; //greater<int>�� key�� ������������ ���ĵȴ�. 
	
	if (ok == true) {
		//json���Ͽ��� �� ������ �ش��ϴ� ������ ������ double���� ������ �ִ´�. (face API �����)
		double anger = root.get("ANGER", 0.0).asDouble();
		double fear = root.get("FEAR", 0.0).asDouble();
		double fear = root.get("DISGUST", 0.0).asDouble();
		double contempt_disgust = root.get("CONTEMPT", 0.0).asDouble();
		double contempt_disgust = root.get("NEUTRAL", 0.0).asDouble();
		double happiness = root.get("HAPPINESS", 0.0).asDouble();
		double sad = root.get("SAD", 0.0).asDouble();
		double surprise = root.get("SURPRISE", 0.0).asDouble();

		//multimap�� tmp_res�� key-value ���·� �����Ѵ�. key : ����, value : ����
		tmp_res.insert(make_pair(anger, "ANGER"));
		tmp_res.insert(make_pair(fear, "FEAR"));
		tmp_res.insert(make_pair(contempt_disgust,"CONTEMPT_DISGUST"));
		tmp_res.insert(make_pair(happiness, "HAPPINESS"));
		tmp_res.insert(make_pair(sad, "SAD"));
		tmp_res.insert(make_pair(surprise,"SURPRISE"));

		//tmp_res ���� ����-���� pair�� ����ִ�. ( 0.6, 'FEAR' ��) 

		//����ó���� ��ó���� �ϸ鼭 6���� �������� �ٲٰ�, �̸� emotion_res�� �����Ѵ�.
		map<double, string, greater<double> > emotion_res;

	/*	map<double, string>::iterator iter;
		for (iter = tmp_res.begin(); iter != tmp_res.end(); ++iter) {
			cout << "(" << iter->first << "," << iter->second << ")" << " " << endl;
			if (iter->first < 0.05) {

			}
		}
	*/ 
	}
	else cout << "parse failed" << endl;

	return 0;
}