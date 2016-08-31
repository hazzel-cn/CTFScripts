#include <mysql.h>
#include <Python.h>
#include <cstring>
#include <string> 
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <cmath>
#include <stack>
#include <algorithm> 
#define MAXN 1000
using namespace std;
class point{
public:
	double x;
	double y;
	int cluster;
	int pid;
        string position;
	int pointType;//1 noise 2 border 3 core
	int pts;//points in MinPts 
	vector<int> corepts;
	int visited;
	point (){}
	point (double a,double b,int c,int pifo,string ption){
		x = a*1000;
		y = b*1000;
		cluster = c;
		pid=pifo;
                position=ption;
	}
};
typedef vector<point> PTARRAY;
typedef vector<point> PTARRAY;
bool operator==(point pt1, point pt2) {

    return (pt1.x == pt2.x && pt1.y == pt2.y);

}

// 姣旇緝涓や釜鍚戦噺pt1鍜宲t2鍒嗗埆涓巟杞村悜�?1, 0)鐨勫す瑙?

bool CompareVector(point pt1,point pt2) {

    //姹傚悜閲忕殑�?

    double m1 = sqrt((double)(pt1.x * pt1.x + pt1.y * pt1.y));

    double m2 = sqrt((double)(pt2.x * pt2.x + pt2.y * pt2.y));

    //涓や釜鍚戦噺鍒嗗埆涓?1, 0)姹傚唴绉?

    double v1 = pt1.x / m1, v2 = pt2.x / m2;

    return (v1 > v2 || (v1 == v2 && m1 < m2));

}

//璁＄畻鍑稿寘

void CalcConvexHull(PTARRAY &vecSrc) {

    //鐐归泦涓嚦灏戝簲鏈?涓偣锛屾墠鑳芥瀯鎴愬杈瑰�?

    if (vecSrc.size() < 3) {

        return;

    }

    //鏌ユ壘鍩虹偣

    point ptBase = vecSrc.front(); //灏嗙�?涓偣棰勮涓烘渶灏忕偣

    for (PTARRAY::iterator i = vecSrc.begin() + 1; i != vecSrc.end(); ++i) {

        //濡傛灉褰撳墠鐐圭殑y鍊煎皬浜庢渶灏忕偣锛屾垨y鍊肩浉绛夛紝x鍊艰緝灏?

        if (i->y < ptBase.y || (i->y == ptBase.y && i->x > ptBase.x)) {

            //灏嗗綋鍓嶇偣浣滀负鏈€灏忕偣

            ptBase = *i;

        }

    }

    //璁＄畻鍑哄悇?��逛�?�鍩虹偣鏋勬垚鐨勫悜�?

    for (PTARRAY::iterator i = vecSrc.begin(); i != vecSrc.end();) {

        //鎺掗櫎涓庡熀鐐圭浉鍚岀殑鐐癸紝閬垮厤鍚庨潰鐨勬帓搴忚绠椾腑鍑虹幇�?閿欒�?

        if (*i == ptBase) {

            i = vecSrc.erase(i);

        }

        else {

            //鏂瑰悜鐢卞熀鐐瑰埌鐩爣�?

            i->x -= ptBase.x, i->y -= ptBase.y;

            ++i;

        }

    }

    //鎸夊悇鍚戦噺涓庢í鍧愭爣涔嬮棿鐨勫す瑙掓帓�?

    sort(vecSrc.begin(), vecSrc.end(), &CompareVector);

    //鍒犻櫎鐩稿悓鐨勫悜閲?

    vecSrc.erase(unique(vecSrc.begin(), vecSrc.end()), vecSrc.end());

    //璁＄畻寰楀埌棣栧熬渚濇鐩歌仈鐨勫悜閲?

    for (PTARRAY::reverse_iterator ri = vecSrc.rbegin();

        ri != vecSrc.rend() - 1; ++ri) {

        PTARRAY::reverse_iterator riNext = ri + 1;

        //鍚戦噺涓夎褰㈣绠楀叕寮?

        ri->x -= riNext->x, ri->y -= riNext->y;

    }

    //渚濇鍒犻櫎涓嶅湪鍑稿寘涓婄殑鍚戦噺

    for (PTARRAY::iterator i = vecSrc.begin() + 1; i != vecSrc.end(); ++i) {

        //鍥炴函鍒犻櫎鏃嬭浆鏂瑰悜鐩稿弽鐨勫悜閲忥紝浣跨敤澶栫Н鍒ゆ柇鏃嬭浆鏂瑰�?

        for (PTARRAY::iterator iLast = i - 1; iLast != vecSrc.begin();) {

            int v1 = i->x * iLast->y, v2 = i->y * iLast->x;

            //濡傛灉鍙夌Н灏忎簬0锛屽垯鏃犳病鏈夐€嗗悜鏃嬭浆

            //濡傛灉鍙夌Н绛変簬0锛岃繕闇€鍒ゆ柇鏂瑰悜鏄惁鐩搁�?

            if (v1 < v2 || (v1 == v2 && i->x * iLast->x > 0 &&

                i->y * iLast->y > 0)) {

                    break;

            }

            //鍒犻櫎鍓嶄竴涓悜閲忓悗锛岄渶鏇存柊褰撳墠鍚戦噺锛屼笌鍓嶉潰鐨勫悜閲忛灏剧浉杩?

            //鍚戦噺涓夎褰㈣绠楀叕寮?

            i->x += iLast->x, i->y += iLast->y;

            iLast = (i = vecSrc.erase(iLast)) - 1;

        }

    }

    //灏嗘墍鏈夐灏剧浉杩炵殑鍚戦噺渚濇绱姞锛屾崲绠楁垚鍧愭爣

    vecSrc.front().x += ptBase.x, vecSrc.front().y += ptBase.y;

    for (PTARRAY::iterator i = vecSrc.begin() + 1; i != vecSrc.end(); ++i) {

        i->x += (i - 1)->x, i->y += (i - 1)->y;

    }

    //娣诲姞鍩虹偣锛屽叏閮ㄧ殑鍑稿寘璁＄畻瀹屾�?
    vecSrc.push_back(ptBase);

}


double stringToFloat(string i){
	stringstream sf;
	double score=0;
	sf<<i;
	sf>>score;
	return score;
}
string floatTostring(double i){
	stringstream ss;
    ss<<i; 
    string s1 = ss.str();
    return s1;
}
string intTostring(int i){
	stringstream ss;
    ss<<i; 
    string s1 = ss.str();
    return s1;
}
int stringToint(string i){
     stringstream sf;
	int score=0;
	sf<<i;
	sf>>score;
	return score;

}
vector<point> openFile(){
	vector<point> data;
	int i=1;
	MYSQL conn;
    int res;
    MYSQL_RES   *resul;
    MYSQL_ROW   row;
    mysql_init(&conn);
    if(mysql_real_connect(&conn,"localhost","root","wyhhfr789","test",3306,NULL,0))
    {
         printf("connect success!\n");
        res=mysql_query(&conn,"update areas set dcount=dcount+1 where dcount>0;");
        if(!res)
            printf("all dount+1\n");
        else
             printf("+1 error");
           mysql_query(&conn,"select id,pointx,pointy,position from areas where dcount>0;");
        resul = mysql_store_result(&conn);
        while(row=mysql_fetch_row(resul)){
            point p(stringToFloat(row[1]),stringToFloat(row[2]),i++,stringToint(row[0]),row[3]);
		    data.push_back(p);
    }
         mysql_close(&conn);
    cout<<"successful!"<<endl;
    }

	else
		printf("connect database error");
	return data;
}

double squareDistance(point a,point b){
	return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}
bool SortByl( const point &v1, const point &v2)//娉ㄦ剰锛氭湰鍑芥暟鐨勫弬鏁扮殑绫诲瀷涓€瀹氳涓巚ector涓厓绱犵殑绫诲瀷涓€鑷? 
{  
    return v1.cluster < v2.cluster;//鍗囧簭鎺掑垪  
}  

void DBSCAN(vector<point> dataset,double Eps,int MinPts){
	int len = dataset.size();
	//calculate pts
	cout<<"calculate pts"<<endl;
	for(int i=0;i<len;i++){
		for(int j=i+1;j<len;j++){
			if(squareDistance(dataset[i],dataset[j])<Eps)
				dataset[i].pts++;
				dataset[j].pts++;
		}
	}
	//core point 
	cout<<"core point "<<endl;
	vector<point> corePoint;
	for(int i=0;i<len;i++){
		if(dataset[i].pts>=MinPts) {
			dataset[i].pointType = 3;
			corePoint.push_back(dataset[i]);
		}
	}
	cout<<"joint core point"<<endl;
	//joint core point
	for(int i=0;i<corePoint.size();i++){
		for(int j=i+1;j<corePoint.size();j++){
			if(squareDistance(corePoint[i],corePoint[j])<Eps){
				corePoint[i].corepts.push_back(j);
				corePoint[j].corepts.push_back(i);
			}
		}
	}
	for(int i=0;i<corePoint.size();i++){
		stack<point*> ps;
		if(corePoint[i].visited == 1) continue;
		ps.push(&corePoint[i]);
		point *v;
		while(!ps.empty()){
			v = ps.top();
			v->visited = 1;
			ps.pop();
			for(int j=0;j<v->corepts.size();j++){
				if(corePoint[v->corepts[j]].visited==1) continue;
				corePoint[v->corepts[j]].cluster = corePoint[i].cluster;
				corePoint[v->corepts[j]].visited = 1;
				ps.push(&corePoint[v->corepts[j]]);				
			}
		}		
	}
	cout<<"border point,joint border point to core point"<<endl;
	//border point,joint border point to core point
	for(int i=0;i<len;i++){
		if(dataset[i].pointType==3) continue;
		for(int j=0;j<corePoint.size();j++){
			if(squareDistance(dataset[i],corePoint[j])<Eps) {
				dataset[i].pointType = 2;
				dataset[i].cluster = corePoint[j].cluster;
				break;
			}
		}
	}

	cout<<"output"<<endl;
	for(int i=0;i<len;i++){
		if(dataset[i].pointType == 2)
			{
			  corePoint.push_back(dataset[i]);
           
			}
             
	}
      
	sort(corePoint.begin(),corePoint.end(),SortByl);
	int pointcount[MAXN]={0};
       
	for(int i=0;i<corePoint.size();i++)
	{
		pointcount[corePoint[i].cluster]++;
	}
       
	int psize=corePoint.size();
	vector<point> probp;
	 int corecount=0;
	  for(int i=0;i<psize;i++)
	      {
         
		if(pointcount[corePoint[i].cluster]<3)
			 corePoint.erase(corePoint.begin()+i);
		else
		{
			corecount++;
			probp.push_back(corePoint[i]);
			if(corecount==pointcount[corePoint[i].cluster])
				{
				
                 corecount=0;
				 MYSQL conn;
                   int res;
                   MYSQL_RES   *resul;
                    mysql_init(&conn);
                    if(mysql_real_connect(&conn,"localhost","root","wyhhfr789","test",3306,NULL,0)){
                         printf("connect success!\n");
                        
                   int is_eval=0;
                   int wuyuhangzuishuai=0;
                 for(int j=0;j<probp.size();j++)
                 {
                     string sql_update = "update areas set dcount=0 where id="+intTostring(probp[j].pid)+";";
                       if(j==probp.size()-1)
                       	 wuyuhangzuishuai=0;
                       	else
                       	  wuyuhangzuishuai=j+1;
                       	
                         if((probp[j].x==probp[wuyuhangzuishuai].x)&&(probp[j].y==probp[wuyuhangzuishuai].y))
                                   is_eval++;
                           res=mysql_query(&conn,sql_update.c_str());
                      
                   }
                         
	             if(is_eval==probp.size())
	             {
	             	string sql_insert1 = "insert into danareas (d_pid,d_px,d_py,position) values ("+intTostring((probp[0].cluster)*10000000+(probp[0].pid))+",'"+floatTostring(probp[0].x)+"','"+floatTostring(probp[0].y)+"','"+probp[0].position+"');";	                      
                    res=mysql_query(&conn,sql_insert1.c_str());
                    if(res)
                       printf("insert danarea error\n");
                    else
                       printf("insert danarea success \n");  
	             } 
	             else{     
				 CalcConvexHull(probp);	 
	            for(int k=0;k<probp.size();k++){
            
	     	         string sql_insert = "insert into danareas (d_pid,d_px,d_py,position) values ("+intTostring((probp[k].cluster)*10000000+(probp[0].pid))+",'"+floatTostring(probp[k].x)+"','"+floatTostring(probp[k].y)+"','"+probp[0].position+"');";	                      
                          res=mysql_query(&conn,sql_insert.c_str());
                            if(res)
                            	printf("insert danarea error\n");
                            else
                            	printf("insert danarea success \n");  

	                      }//..
	                      
                    } }
	                  
	            else
		                printf("connect database error"); 
		           mysql_close(&conn);
			       probp.clear();
				   }
		 }
	  
	//output
    

}
}
extern "C"
int haha() {
	vector<point> dataset = openFile();
        DBSCAN(dataset,10,2);
	return 0;
}
