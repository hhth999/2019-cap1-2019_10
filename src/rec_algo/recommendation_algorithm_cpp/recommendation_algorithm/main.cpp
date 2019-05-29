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

enum Emotion { 
	ANGER,
	CONTEMPT_DISGUST,
	FEAR,
	HAPPINESS,
	SAD,
	SURPRISE
};

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
	int table1, table2, table3;

	/*
	0 : ���� ���̺� �� ANGER
	1 : ���� ���̺� �� CONTEMPT_DISGUST
	2 : ���� ���̺� �� FEAR
	3 : ���� ���̺� �� HAPPINESS
	4 : ���� ���̺� �� SAD
	5 : ���� ���̺� �� SURPRISE
	6 : ������ ���̺�
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

		//tmp_res ���� ����-���� pair�� ����ִ�. ( 0.6, 'FEAR' ��) : raw emotion data

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
				table1 = 6;
				table2 = 6;
				table3 = 6;
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
		 
		int cnt = 0;
		int happiness_rate, surprise_rate; //�˰��� �Ǻ����� ���� �������
		vector<string> rank_emotion;

		/* vector<string> ���� : '���̺� �ε���-�������̺�' ������ ����
		   -> �˰��� ó���� ��, �ε����� �̿��� �� ���� (�ε���-�������̺�) */
		vector<string> emotion_tables; 

		//�ε���= DB�� ���̺� �ε��� & value = DB�� �������̺�
		emotion_tables.push_back("ANGER");
		emotion_tables.push_back("CONTEMPT_DISGUST");
		emotion_tables.push_back("FEAR");
		emotion_tables.push_back("HAPPINESS");
		emotion_tables.push_back("SAD");
		emotion_tables.push_back("SURPRISE");

		//1~3���� ���� �����ϱ����� ���� ���� & rank_emotion�� ����
		for (auto &iter : emotion_res) {
			cout << iter.first << " " << iter.second << endl; //�����
			if (iter.first != 0) {
				rank_emotion.push_back(iter.second);
				cnt++;
			}
			if (iter.second == "HAPPINESS") {
				happiness_rate = iter.first;
			}
			if (iter.second == "SURPRISE") {
				surprise_rate = iter.first;
			}
		}

		//3���� �������� ���ڶ��κ� ä���
		if (cnt == 1) { //1���� ���������� �ִٸ�
			//2����, 3���� ������ 1���� �������� ä��
			rank_emotion.push_back(rank_emotion[0]);
			rank_emotion.push_back(rank_emotion[0]);
		}
		else if (cnt == 2) { //2���� ���������� �ִٸ�
			//3���� ������ 1���� �������� ä��
			rank_emotion.push_back(rank_emotion[0]);
		}
		else if (cnt >= 3) { //3���� ������ �ִٸ�
			cout << "3�������� �ִ�" << endl;
		}

		cout << endl << endl;
		cout << "����� �ѤѤѤ�" << endl;
		cout << "cnt : " << cnt << endl;
		cout << rank_emotion[0] << " " << rank_emotion[1] << " " << rank_emotion[2] << endl;
		cout << endl << endl;

		/*
		�ε��� - ���̺�
		0 : ���� ���̺��� ANGER (ȭ)
		1 : ���� ���̺��� CONTEMPT_DISGUST (���_���ܿ�)

		2 : ���� ���̺��� FEAR (����)
		3 : ���� ���̺��� HAPPINESS (�ູ)
		
		4 : ���� ���̺��� SAD (����)
		5 : ���� ���̺��� SURPRISE (���)
		
		6 : ������ ���̺�
		*/
		
		if (rank_emotion[0] == "ANGER") { //ȭ
			table1 = 41; // 4:�������̺� 1:����(1)����(����)
			table2 = 0;  // 0:ȭ�����̺�
			
			//table3 : '2���� ����'�� �ش��ϴ� ���� ���̺�
			for (int i = 0; i < emotion_tables.size(); i++) {
				if (emotion_tables[i] == rank_emotion[1]) {
					table3 = i;
					if (table3 == 4) table3 = 42;
				}
			} 
		}
		else if (rank_emotion[0] == "FEAR") { //����
			table1 = 2; //2:�������̺�
			table2 = 41;

			//table3 : '2���� ����'�� �ش��ϴ� ���� ���̺�
			for (int i = 0; i < emotion_tables.size(); i++) {
				if (emotion_tables[i] == rank_emotion[1]) {
					table3 = i;
					if (table3 == 4) table3 = 42;
				}
			}
		}
		else if (rank_emotion[0] == "HAPPINESS") { //�ູ
			if (happiness_rate >= 0.8) {
				table1 = 3; //3:�ູ���̺�
				table2 = 3;
				table3 = 3;
			}
			else {
				table1 = 3;
				
				//table2 : '2���� ����'�� �ش��ϴ� ���� ���̺�
				for (int i = 0; i < emotion_tables.size(); i++) {
					if (emotion_tables[i] == rank_emotion[1]) {
						table2 = i;
						if (table2 == 4) table2 = 42;
					}
				}

				//table3 : '3���� ����'�� �ش��ϴ� ���� ���̺�
				for (int i = 0; i < emotion_tables.size(); i++) {
					if (emotion_tables[i] == rank_emotion[2]) {
						table3 = i;
						if (table3 == 4) table3 = 42;
					}
				}
			}
		}
		else if (rank_emotion[0] == "CONTEMPT_DISGUST") { //���_���ܿ�
			table1 = 41;
			table2 = 1;
			
			//table3 : '3���� ����'�� �ش��ϴ� ���� ���̺�
			for (int i = 0; i < emotion_tables.size(); i++) {
				if (emotion_tables[i] == rank_emotion[2]) {
					table3 = i;
					if (table3 == 4) table3 = 42;
				}
			}
		}
		else if (rank_emotion[0] == "SAD") { //����
			table1 = 42; // 4:�������̺�, 2:����(2)����(�ʹ����ۼ� ������)
			table2 = 41; // 4:�������̺�, 1:����(1)����(����)
			table3 = 43; // 4:�������̺�, 3:����(3)����(�����ȯ)
		}
		else { //���
			if (surprise_rate >= 0.8) {
				table1 = 5; //5:������̺�
				table2 = 5;
				table3 = 5;
			}
			else {
				table1 = 5;

				//table2 : '2���� ����'�� �ش��ϴ� ���� ���̺�
				for (int i = 0; i < emotion_tables.size(); i++) {
					if (emotion_tables[i] == rank_emotion[1]) {
						table2 = i;
						if (table2 == 4) table2 = 42;
					}
				}

				//table3 : '3���� ����'�� �ش��ϴ� ���� ���̺�
				for (int i = 0; i < emotion_tables.size(); i++) {
					if (emotion_tables[i] == rank_emotion[2]) {
						table3 = i;
						if (table3 == 4) table3 = 42;
					}
				}
			}

		}

		cout << "����� ---------------------------------" << endl;
		cout << rank_emotion[0] << " " << rank_emotion[1] << " " << rank_emotion[2] << endl;
		cout << "���õ� table : "<<table1 << " " << table2 << " " << table3 << endl;
		//table1,2,3���� ���ü��ִ� ���� : 1/2/3/ 41/42/43 / 5 / 6(������)
	}
	else cout << "parse failed" << endl; 
	return 0;
}