#include<bits/stdc++.h>
#include<windows.h>
#include<io.h>
using namespace std;

int tag=0;

//structure
struct wd {
	int num;
	char *wordname;
	wd *next;
};
char *get(wd *head,int i) {
	wd *p;
	p=head;
	for(;i>0;i--) p=p->next;
	return p->wordname;
}
char *del(wd *head,int i) {
	wd *p;
	p=head;
	for(;i>1;i--) p=p->next;
	p->next=p->next->next;
	head->num--;
}

//download 
void download(char* urlname,char* filename,int mode=0) {
	FILE *vbs;
	vbs=fopen("download.vbs","w");
	fprintf(vbs,"On Error Resume Next \n");
	fprintf(vbs,"Set Post = CreateObject(\"Msxml2.XMLHTTP\") \n");
	fprintf(vbs,"Set Shell = CreateObject(\"Wscript.Shell\") \n");
	fprintf(vbs,"Post.Open ");
	fprintf(vbs,"\"GET\",\"%s\",0\n",urlname);
	fprintf(vbs,"Post.Send()\n");
	fprintf(vbs,"Set aGet = CreateObject(\"ADODB.Stream\")\n");
	fprintf(vbs,"aGet.Mode = 3\n");
	fprintf(vbs,"aGet.Type = 1\n");
	fprintf(vbs,"aGet.Open()\n");
	fprintf(vbs,"aGet.Write(Post.responseBody)\n");
	fprintf(vbs,"aGet.SaveToFile \"%s\",2\n",filename);
	fclose(vbs);
	if(mode==0) system("download.vbs");
	if(mode==1) system("start download.vbs");
	system("del download.vbs");
}

//check
bool checkstring(FILE *tasklist,char* s) {
	char c;
	bool b;
	int n=strlen(s);
	while(fscanf(tasklist,"%c",&c)!=EOF) {
		//cout<<c;
		if(c==*s) {
			b=1;
			for(int i=0; i<=n-1; i++) {
				//cout<<"i="<<i;
				if(c!=*(s+i)) {
					b=0;
					break;
				}
				if(fscanf(tasklist,"%c",&c)==EOF) b=0;
				//cout<<c<<endl;
			}
			if(b==1) return 1;
		}
	}
	return 0;
}
int checkclass(FILE *filearray) {
	char classname[51];
	//classname=new char [50];
	for(int i=0; i<=50; i++) {
		classname[i]='\0';
	}
	int i=0;
	char c;
	while(fscanf(filearray,"%c",&c)!=EOF) {
		cout<<c;
		if(c=='"') break;
		classname[i++]=c;
	}
	if(strcmp(classname,"each_seg")==0) {//form
		tag=0;
		return 0;
	}
	if(tag==1) return 7;
	if(strcmp(classname,"dis")==0) return 1;//class
	if(strcmp(classname,"se_lis")==0) return 2;//terms
	if(strcmp(classname,"se_d b_primtxt")==0) return 3;//num
	if(strcmp(classname,"au_def")==0) return 4;
	if(strcmp(classname,"sen_com")==0) return 5;
	if(strcmp(classname,"def_pa")==0) return 6;
	if(strcmp(classname,"li_id b_divdef")==0) {
		tag=1;
		return 7;
	}
	if(strcmp(classname,"de_li1 de_li3")==0) return 200;
	return 255;

}

//work
void deal_oxford(const char *word,FILE *output){
	//declearation
	FILE *html;
	char url[101],file[101];
	char c,c0,o;
	
	//start
	fprintf(output,"<div class=\"each_word\">");
	
	//get data
	sprintf(file,"html\\%s_bing.html",word);
	html=fopen(file,"r");
	
	//check existence
	if(checkstring(html,"<div class=\"li_sen\" id=\"newLeId\"")) {
		//wordtitle
		fprintf(output,"<div class=\"hw_area2\"><div class=\"hd_div2\"><span class=\"hw\">%s</span></div></div>",word);

		while(checkstring(html,"<div class=")) {
			int checkclassi=checkclass(html);
			cout<<checkclassi;
			if(checkclassi==0) {//form
				checkstring(html,"<div class=\"pos\"");
				fprintf(output,"<div class=\"pos\">",c);
				while(fscanf(html,"%c",&c)!=EOF) {
					if(c=='<') break;
					if(c<=0||(c>='a'&&c<='z')) fprintf(output,"%c",c);
				}
				fprintf(output,"</div>");
			}
			if(checkclassi==1) { //class
				fprintf(output,"<div class=\"dis\"><span class=\"bil_dis b_primtxt\">");
				checkstring(html,"<span class=\"bil_dis b_primtxt\"");
				while(fscanf(html,"%c",&c)!=EOF) {
					if(c=='<') break;
					if(c<=0||(c>='a'&&c<='z')||c=='/'||c==' ') fprintf(output,"%c",c);
				}
				fprintf(output,"</span><span class=\"val_dis b_primtxt\">");
				checkstring(html,"<span class=\"val_dis b_primtxt\"");
				while(fscanf(html,"%c",&c)!=EOF) {
					if(c=='<') break;
					if(c<=0||(c>='a'&&c<='z')||c=='/'||c==' ') fprintf(output,"%c",c);
				}
				fprintf(output,"</span></div>");
			}
			if(checkclassi==2) { //terms
				/**<div class="se_lis"><table><tbody><tr class="def_row"><td><div class="se_d b_primtxt">1.</div></td><td><div class="de_co"><div class="au_def"><span class="gra b_regtxt">[t]</span></div><div class="au_def"></div><div class="sen_com"><span class="comple b_regtxt">~ sth</span></div><div class="def_pa"><span class="bil b_primtxt">吸引住（注意力、兴趣）</span><span class="val b_regtxt">to succeed in attracting and keeping sb's attention and interest</span></div></div></td></tr></tbody></table></div>*/
				fprintf(output,"<div class=\"se_lis\"><table><tbody><tr class=\"def_row\">");

			}
			if(checkclassi==3) {//num
				fprintf(output,"<td><div class=\"se_d b_primtxt\">");
				while(fscanf(html,"%c",&c)!=EOF) {
					if(c=='<') break;
					if(c<=0||(c>='0'&&c<='9')||c=='.') fprintf(output,"%c",c);

				}
				fprintf(output,"</div></td>");
				fprintf(output,"<td><div class=\"de_co\">");

			}
			if(checkclassi==4) { //guide
				fscanf(html,"%c",&c);
//				cout<<"here["<<c<<"]";
				fscanf(html,"%c",&c);
//				cout<<"here["<<c<<"]";
				fscanf(html,"%c",&c);
//				cout<<"here["<<c<<"]";
				if(c=='s') {
					checkstring(html,"\"");
					checkstring(html,"\"");
					fprintf(output,"<div class=\"au_def\"><span class=\"gra b_regtxt\">");
					while(fscanf(html,"%c",&c)!=EOF) {
						if(c=='<') break;
						fprintf(output,"%c",c);
					}
					fprintf(output,"</span></div>");
				}

			}
			if(checkclassi==5) { //usage
				fprintf(output,"<div class=\"sen_com\"><span class=\"comple b_regtxt\">");
				checkstring(html,"<span class=\"comple b_regtxt\"");
				while(fscanf(html,"%c",&c)!=EOF) {
					if(c=='<') break;
					fprintf(output,"%c",c);
				}
				fprintf(output,"</span></div>");
			}
			if(checkclassi==6) { //translation
				fprintf(output,"<div class=\"def_pa\"><span class=\"bil b_primtxt\">");
				checkstring(html,"<span class=\"bil b_primtxt\"");
				while(fscanf(html,"%c",&c)!=EOF) {
					if(c=='<') break;
					fprintf(output,"%c",c);
				}
				fprintf(output,"</span><span class=\"val b_regtxt\"></span></div>");
				checkstring(html,"<span class=\"val b_regtxt\"");
				while(fscanf(html,"%c",&c)!=EOF) {
					if(c=='<') break;
					fprintf(output,"%c",c);
				}
				fprintf(output,"</span></div></td></tr></tbody></table></div>");
			}
			
			if(checkclassi==200) {
				cout<<"break"<<endl;
				break;
			}
		}
		fclose(html);
	}
	
	//end
	fprintf(output,"</div>");
}

int main() {
	//declearation
	FILE *input,*html,*output,*tmp,*example;
	char *word,c,c0,o;
	word=new char[51];
	char url[101],file[101];
	int letter=0,num=0;
	
	//structure
	wd *h,*p,*r;
	h=new wd;
	r=h;
	h->num=0;
	int rd,n=1;
	
	//input wordlist
	input=fopen("input.txt","r");
	while(fgets(word,50,input)) {
		p=new wd;
		p->num=n;
		p->wordname=new char[51];
		for(int i=0; i<=50; i++) {
			if(*(word+i)=='\n') *(p->wordname+i)='\0';
			else *(p->wordname+i)=*(word+i);
		}
		p->next=NULL;
		r->next=p;
		r=p;
		n++;
		h->num++;
	}
	fclose(input);
	
	//cout
	cout<<"read word finished..."<<endl;
	for(int i=1;i<=h->num;i++){
		cout<<get(h,i)<<endl;
	} 
	cout<<endl;
//	getch();
	
	//download
	cout<<"downloading started..."<<endl;
	for(int i=1;i<=h->num;i++){
		sprintf(file,"html\\%s_bing.html",get(h,i));
		if(access(file,0)==0){
			cout<<"exist:"<<get(h,i)<<endl;
			continue;
		}
		
		cout<<"downloading:"<<get(h,i)<<endl;
		sprintf(url,"https://cn.bing.com/dict/search?q=%s",get(h,i));
		sprintf(file,"html\\%s_bing.html",get(h,i));
		download(url,file);
	}
	cout<<"downloading finished."<<endl<<endl;
	
	
	
	//deal
	cout<<"dealing started..."<<endl;
	for(int i=1;i<=h->num;i++){
		word=get(h,i);
		cout<<"dealing word:"<<word<<endl;
		
		//prepare
		cout<<"head writing started..."<<endl;
		time_t now = time(0);
		tm *ltm = localtime(&now);
		sprintf(file,"dict\\%s.html",word);
		
		//deal 
		output=fopen(file,"w");
		example=fopen("example.html","r");
		while(fscanf(example,"%c",&c)!=EOF) fprintf(output,"%c",c);
		cout<<"head writing finished."<<endl<<endl;
		deal_oxford(word,output);
		cout<<endl;
		
		//end
 		cout<<"end writing started..."<<endl;
		fprintf(output,"</body></html>");
		fclose(output);
//		sprintf(file,"start wordoutput%04d%02d%02d%02d%02d%02d.html",ltm->tm_year+1900,ltm->tm_mon+1,ltm->tm_mday,ltm->tm_hour,ltm->tm_min,ltm->tm_sec);
		system(file);
		cout<<"end writing finished.";
	}
	cout<<"dealing finished."<<endl<<endl;
	
	
	
}

