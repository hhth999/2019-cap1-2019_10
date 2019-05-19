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

	multimap<double, string, greater <int>> emotion_res; //greater<int>�� key�� ������������ ���ĵȴ�. 
	
	if (ok == true) {
		//json���Ͽ��� �� ������ �ش��ϴ� ������ ������ double���� ������ �ִ´�. 
		double anger = root.get("ANGER", 0.0).asDouble();
		double fear = root.get("FEAR", 0.0).asDouble();
		double contempt_disgust = root.get("CONTEMPT_DISGUST", 0.0).asDouble();
		double happiness = root.get("HAPPINESS", 0.0).asDouble();
		double sad = root.get("SAD", 0.0).asDouble();
		double surprise = root.get("SURPRISE", 0.0).asDouble();

		//multimap�� emotion_res�� key-value ���·� �����Ѵ�. key : ����, value : ����
		emotion_res.insert(make_pair(anger, "ANGER"));
		emotion_res.insert(make_pair(fear, "FEAR"));
		emotion_res.insert(make_pair(contempt_disgust,"CONTEMPT_DISGUST"));
		emotion_res.insert(make_pair(happiness, "HAPPINESS"));
		emotion_res.insert(make_pair(sad, "SAD"));
		emotion_res.insert(make_pair(surprise,"SURPRISE"));

		//�����-��� 
		for (auto &it : emotion_res) {
			cout << it.first << " " << it.second << endl;
		}

	}
	else cout << "parse failed" << endl;

	return 0;
}