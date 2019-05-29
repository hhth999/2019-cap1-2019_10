# -*- coding: cp949 -*-
import json
from random import *
import operator

'''parameter format (1) face : dictionary
                        anger neutral fear happiness sadness / surprise contempt disgust
                    (2) tone : string
                        angry calm fearful happy sad 
    *�������� ��� : �ູ<->���ĸ� ����������
                   ������� 1���� �� ����� ������ (�߸��� ������ �� �� ����)
                   �ϵ��ڵ����� �ູ = [��,��] , ���� = [��, ��] ,,, �̷��� ���̽� �� �־����
                   ������� 1���� �� ��� 1������ ���ϸ� ?
                => �츮�� ���ϱ� �����̱� �ѵ�, �ϴ� �ູ <->���ĸ� �ְ� �׽�Ʈ�ϱ��  

    * 0 : ���� ���̺� �� ANGER
      1 : ���� ���̺� �� CONTEMPT_DISGUST
      2 : ���� ���̺� �� FEAR
      3 : ���� ���̺� �� HAPPINESS
      4 : ���� ���̺� �� SADNESS
      5 : ���� ���̺� �� SURPRISE
      6 : ������ ���̺�
     ** �������̺��� ���ɸ� �ʿ��ϹǷ�, ���̽� �������� �� ���� ������ �˻��ϰ� ������ �����̸� ��õ�˰����� �ʿ����.
'''

def recom(face, tone):
    #���̺� �ε����� ����Ʈ�� �����Ѵ�.
    table_idx = ["ANGER", "CONTEMPT_DISGUST", "FEAR", "HAPPINESS", "SADNESS", "SURPRISE"]
    table1 = 100
    table2 = 100
    table3 = 100
 
    #tone ����� face ������Ŀ� ������ : angry->anger / calm->neutral / fearful->fear / happy->happiness / sad->sadness
    tone_res = ""
    if tone == "angry" :
        tone_res = "ANGER"
    elif tone == "calm" :
        tone_res = "NEUTRAL"
    elif tone == "fearful" :
        tone_res = "FEAR"
    elif tone == "happy" :
        tone_res = "HAPPINESS"
    elif tone == "sad" :
        tone_res = "SADNESS"
    else :
        print("tone_res err")

    #parse json data (input format : .json file)
    with open("test.json") as json_file: 
        dic = json.load(json_file)

        anger = dic["ANGER"]
        happiness = dic["HAPPINESS"]
        sadness = dic["SADNESS"]
        neutral = dic["NEUTRAL"]
        fear = dic["FEAR"]
        surprise = dic["SURPRISE"]
        contempt = dic["CONTEMPT"]
        disgust = dic["DISGUST"]

        print(dic)
 
    #��ó�� - 6���� �������� ����� ����, �켱 contempt�� disgust�� ��ģ��. 
    contempt_disgust = contempt + disgust
    dic["CONTEMPT_DISGUST"] = contempt_disgust
    #print(dic)

    #�߸��� 0.8 �̻��̸� 3���� ���� ���̺��� �����Ѵ�. (0���� 5���� �� ���� ����)
    if neutral > 0.8 :
        table1 = random.randrange(0,6) 
        table2 = random.randrange(0,6) 
        table3 = random.randrange(0,6)
        return table1, table2, table3
        
    #��ó�� - 0.05�̸��� ���� 0.0���� �������Ѵ�. 
    else :
        for emotion, rate in dic.items() :
            if rate < 0.05 :
                dic[emotion] = 0.0 
        #print(dic)

    #��ó�� - �������� ���, ������ ���̺� ����
    if tone_res == "SADNESS" and happiness > 0 :
        print("�ູ �󱼷� ���� ��Ҹ��� ���� ����������1")
        table1 = 6
        table2 = 6
        table3 = 6 
        return table1, table2, table3
        
    elif tone_res == "HAPPINESS" and sadness > 0 :
        print("���� �󱼷� �ູ ��Ҹ��� ���� ����������2")
        table1 = 6
        table2 = 6
        table3 = 6
        return table1, table2, table3

    #��ó�� (1) �߸��� ���� ���� ���̺� (2) ���������̺� -> �� �� �ش����� ������ 1,2,3���� ������ ���� �˰��� ó�� ����  
    else :
        del dic['NEUTRAL']
        del dic["CONTEMPT"]
        del dic["DISGUST"]

        emotion_res = sorted(dic.items(), key=operator.itemgetter(1), reverse=True) #value ���� �������� ����
        #print("������")
        #print(emotion_res)

        rank_emotion = [] #�ε��� ������� ���� ���� �����̴�. 3���� �������� �����Ѵ�.
        rank_emotion_cnt = 0
        for emotion, rate in emotion_res :
            if rate > 0 :
                rank_emotion.append(emotion)
                rank_emotion_cnt += 1
                if rank_emotion_cnt > 3 :
                    break

        #print(rank_emotion)
        if rank_emotion_cnt == 1 :
            rank_emotion.append(rank_emotion[0])
            rank_emotion.append(rank_emotion[0])
        elif rank_emotion_cnt == 2:
            rank_emotion.append(rank_emotion[0])
                
        #print(rank_emotion)

        if rank_emotion[0] == "ANGER" :
            table1 = 41 #4:�������̺� 41:����(1)����(����)  
            table2 = 0  #0:ȭ�����̺�
            
            #table3 : '2���� ����'�� �ش��ϴ� ���� ���̺�
            table3 = table_idx.index(rank_emotion[1])
            if table3 == 4 : #2������ SADNESS���, 41�� ����(1)����(����)�� �����Ѵ�. 
                table3 = 41

        elif rank_emotion[0] == "FEAR" :
            table1 = 2 #2:�������̺�
            table2 = 41 #����(1)����(����)
            
            #table3 : 2���� ������ �ش��ϴ� ���� ���̺�
            table3 = table_idx.index(rank_emotion[1])
            if table3 == 4 :
                table3 = 41

        elif rank_emotion[0] == "HAPPINESS" :
            if happiness >= 0.8 :
                table1 = 3 #3:�ູ���̺�
                table2 = 3
                table3 = 3 
            else :
                table1 = 3
                
                #table2 : 2���� ������ �ش��ϴ� ���� ���̺�
                table2 = table_idx.index(rank_emotion[1])
                if table2 == 4 :
                    table2 = 41

                #table3 : 3���� ������ �ش��ϴ� ���� ���̺�
                table3 = table_idx.index(rank_emotion[2])
                if table3 == 4 :
                    table3 = 41

        elif rank_emotion[0] == "CONTEMPT_DISGUST" :
            table1 = 41 #����(1)����(����)
            table2 = 1  #1:���_���ܿ� ���̺�
            
            #table3 : 2���� ������ �ش��ϴ� ���� ���̺�
            table3 = table_idx.index(rank_emotion[1])
            if table3 == 4 :
                table3 = 41

        elif rank_emotion[0] == "SAD" :
            table1 = 42 #����(2)����(�ʹ����ۼ� ������)
            table2 = 41 #����(1)����(����)
            table3 = 43 #����(3)����(�����ȯ)

        elif rank_emotion[0] == "SURPRISE" :
            if SURPRISE >= 0.8 :
                table1 = 5 #5:������̺�
                table2 = 5
                table3 = 5 
            else :
                table1 = 5
                
                #table2 : 2���� ������ �ش��ϴ� ���� ���̺�
                table2 = table_idx.index(rank_emotion[1])
                if table2 == 4 :
                    table2 = 41

                #table3 : 3���� ������ �ش��ϴ� ���� ���̺�
                table3 = table_idx.index(rank_emotion[2])
                if table3 == 4 :
                    table3 = 41
                    
    return table1, table2, table3

if __name__ == "__main__" :
    #recom(1,1)
    db_table1, db_table2, db_table3 = recom(1,"angry") # json������ ù��° �Ķ���ͷ� �ְ� ���� 
    print(db_table1)
    print(db_table2)
    print(db_table3)
    
