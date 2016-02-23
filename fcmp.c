#include<stdio.h>
#include<string.h>

void compress(FILE* fp,char* fileName){
	unsigned char cnt[10000]={0,};
    FILE* fpw;
    unsigned char tmp;
	unsigned char zCnt=0;
	unsigned char nu='\0';
	unsigned char mgNum[4];
	short i=0;
	int j;

    fpw = fopen(fileName,"wb");
	
	fputs("KMzip.",fpw);
	fread(mgNum,sizeof(unsigned char),4,fp);
	if(mgNum[0]==0x7f && mgNum[1]=='E' && mgNum[2]=='L' && mgNum[3]=='F'){
		fputc('1',fpw);
		fputc('.',fpw);	
	}
	else{
		fputc('2',fpw);
		fputc('.',fpw);
		fseek(fp,0,SEEK_SET);
	}
	
    while(!feof(fp)){
		tmp = (unsigned char)fgetc(fp);
		
		if(tmp!='\0')
	        fputc(tmp,fpw);
		else{
			while(tmp=='\0'){
				zCnt++;
				if(zCnt==255){
					fputc('\0',fpw);
					tmp = (unsigned char)fgetc(fp);
					cnt[i]=zCnt;
					zCnt=0;
					i++;
					continue;
				}
				tmp = (unsigned char)fgetc(fp);
			}
			fputc('\0',fpw);
			fputc(tmp,fpw);
			cnt[i]=zCnt;
			zCnt=0;
			i++;
		}	
    }	
	for(j=0;j<i;j++){
		fputc(cnt[j],fpw);
	}
	fputc('\0',fpw);
	fprintf(fpw,"%6d",i);
	printf("%d\n",i);
    fclose(fpw);
}

void decompress(FILE* fp, char* fileName){
//ff(eof)가 나오면 그 뒤부터는 \0의 개수를 저장한 배열
	FILE* fpw;
	long int pos;
	unsigned char cnt[10000]={0,};
	unsigned char tmp;
	int i=0;
	unsigned char j;
	int len=0;
	char chmod[100]="chmod 755 ";
	unsigned char form[10];
	int formSel;
		
	fread(form,sizeof(unsigned char),8,fp);
	if(form[0]!='K'||form[1]!='M'||form[2]!='z'||form[3]!='i'||
		form[4]!='p'||form[5]!='.'){
		printf("I can't decompress this file\n");
		return;
	}
	fpw = fopen(fileName,"wb");

	if(form[6]=='1' && form[7]=='.'){
		formSel=1;
		fputc(0x7f,fpw);
		fputc('E',fpw);
		fputc('L',fpw);
		fputc('F',fpw);
	}
	else
		formSel=2;	

	fseek(fp,-6,SEEK_END);
	fscanf(fp,"%d",&len);
	printf("%d\n",len);

	fseek(fp,-8-len,SEEK_END);
	pos = ftell(fp);
	fgetc(fp);

	for(i=0;i<len;i++){
		tmp=(unsigned char) fgetc(fp);
		cnt[i]=tmp;
	}

	fseek(fp,8,SEEK_SET);
	
	i=0;
	tmp=0;
	while(ftell(fp)!=pos){
		tmp=(unsigned char)fgetc(fp);
		if(tmp=='\0'){
			for(j=0;j<cnt[i];j++){
				fputc('\0',fpw);
			}
			i++;
		}
		else{
			fputc(tmp,fpw);
		}
	}
	fclose(fpw);
	if(formSel==1){
		strcat(chmod,fileName);
		system(chmod);
	}
	
}


int main(int argc, char** argv){
	FILE* fp;
	fp = fopen(argv[2],"rb");
	if(fp==NULL){
		printf("file input error");
		return 0;
	}
	printf("file input OK\n");
	if(argv[1][0]=='a'){
		compress(fp,argv[3]);
	}
	else if(argv[1][0]=='b'){
		decompress(fp,argv[3]);
	}
	else{
		printf("you just activated my trap card\n");
	}
	fclose(fp);
    return 0;
}



