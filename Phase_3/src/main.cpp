#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include<string.h>
#define initial 268435456
using namespace std;
int registers[32],ilo;
bool mc= false;
map<int ,string> memory;
string immp="",rry="";
map<int,string> instr_memory;
map<int,string> memory_i;
string binary;
vector<int> finalreg;
int i=0,r,r1,r2,rz=0,clocks=0,imn,ka=0,sy,programCounter=0,kac=0,cycles=0,stalld=0,countpp=0,choicep,choiced,onebitpredictor, hit=0, miss=0, cunt=0, jali=0, beqs=0,  check=0, chk=0, jalsi=0, falg=0, ad1=0, ad2=0, ad3=0, a001=0, a002=0, a003=0, a004=0, a005=0, a006=0, a007=0,nls=0,nci=0,nai=0,dh=0,stallc=0,choicer,choicepipe,choicepipe2,innum;
long long int val=0;
bool subtr=false;
struct onebitpred{
    //int programCounter
    int bht;
    int targetadd;
};
onebitpred  pred[10000];

class instr
{
public:
    string instruction;
    string operation;
    int reg1;
    int reg2;
    int value;
    long long int val;
    int yy;
    int final_reg;
    int stage;
    int reg1value;
    int reg2value;
    int final_regvalue;
    int value_imm;
    int didstall;
    int progcount;
    bool exec;
    bool stalled;
};

instr pipelining[10000];


string lookup(string word)
{
    if(word=="0") return "0000";
    if(word=="1") return "0001";
    if(word=="2") return "0010";
    if(word=="3") return "0011";
    if(word=="4") return "0100";
    if(word=="5") return "0101";
    if(word=="6") return "0110";
    if(word=="7") return "0111";
    if(word=="8") return "1000";
    if(word=="9") return "1001";
    if(word=="A") return "1010";
    if(word=="B") return "1011";
    if(word=="C") return "1100";
    if(word=="D") return "1101";
    if(word=="E") return "1110";
    if(word=="F") return "1111";
    else return "";
}

string tookup(int word)
{
    if(word==0) return "0";
    if(word==1) return "1";
    if(word==2) return "2";
    if(word==3) return "3";
    if(word==4) return "4";
    if(word==5) return "5";
    if(word==6) return "6";
    if(word==7) return "7";
    if(word==8) return "8";
    if(word==9) return "9";
    if(word==10) return "A";
    if(word==11) return "B";
    if(word==12) return "C";
    if(word==13) return "D";
    if(word==14) return "E";
    if(word==15) return "F";
    else return "";
}

int bin_to_dec(string r)
{
    int i,j=1,num=0;
    for(i=r.length()-1;i>=0;i--)
    {
        num=num+((r[i]-48)*j);
        j=j*2;
    }
    return num;
}

string dec_to_hex(int n){
    int k=n; string ans="";
    if(k==0){
        return "0x00000000";
    }
    while(k!=0){

            ans = tookup(k%16)+ans;
            k=k/16;
       
    }
    int lp = ans.size();
    if(lp<8){
        for(int i=0; i<8-lp; i++){
            ans = "0"+ans;
        }
    }
    else if(lp>8){
        ans = ans.substr(lp-8,8);
    }
    ans = "0x"+ans;
    return ans;
}

int hex_to_dec(string value)
{
    int result=0,num=1;
    for(i=value.length()-1;i>=0;i--)
    {
        if(value[i]>=48&&value[i]<=57)
            value[i]=value[i]-48;
        else if(value[i]>=65&&value[i]<=70)
            value[i]=value[i]-55;
        else if(value[i]>=97&&value[i]<=102)
            value[i]=value[i]-87;
        result=result+(value[i]*num);
        num=num*16;
    }
    return result;
}

int hex_to_dec_ad(string value)
{
    int result=0,num=1;
    for(i=value.length()-1;i>1;i--)
    {
        if(value[i]>=48&&value[i]<=57)
            value[i]=value[i]-48;
        else if(value[i]>=65&&value[i]<=70)
            value[i]=value[i]-55;
        else if(value[i]>=97&&value[i]<=102)
            value[i]=value[i]-87;
        result=result+(value[i]*num);
        num=num*16;
    }
    return result;
}

int hex_to_dec_sp(string value)
{
    return ((value[0]*16)+value[1]);
}

string decode_sb(string binary,string opcode)
{
    string imm4=binary.substr(0,1);
    string imm3=binary.substr(1,6);
    string rs2=binary.substr(7,5);
    string rs1=binary.substr(12,5);
    string func3=binary.substr(17,3);
    string imm2=binary.substr(20,4);
    string imm1=binary.substr(24,1);
    string imm = imm4+imm1+imm3+imm2+"0";
    string op="";
    if(opcode=="1100011"&&func3=="000")
        op="beq";
    else if(opcode=="1100011"&&func3=="001")
        op="bne";
    else if(opcode=="1100011"&&func3=="100")
        op="blt";
    else if(opcode=="1100011"&&func3=="101")
        op="bge";
    r1=bin_to_dec(rs1);
    r2=bin_to_dec(rs2);
    r=-1;
    if(imm[0]=='0')
    {
        imn=bin_to_dec(imm);
    }
    else
    {
        int swapping=0;
        for(int i=12;i>=0;i--)
        {
            if(swapping==1)
            {
                if(imm[i]=='1')
                {
                    imm[i]='0';
                    continue;
                }
                if(imm[i]=='0')
                {
                    imm[i]='1';
                    continue;
                }

            }
            if(imm[i]=='1')
            {
                swapping=1;
            }

        }
       imn=bin_to_dec(imm);
        imn=imn*(-1);
    }
    
    cout<<"DECODE: Operation is "<<op<<", "<<"first operand R"<<r2<<", second operand R"<<r1<<endl;
    cout<<"DECODE: Read registers R"<<r2<<" = "<<registers[r2]<<", R"<<r1<<" = "<<registers[r1]<<endl;
    cout<<imn<<"\n";
    return op;
}

string decode_r(string binary)
{
    string func3=binary.substr(17,3);
    string func7=binary.substr(0,7);
    string op="";
    if(func3=="000"&&func7=="0000000"){
        op="add"; a006++;}
    else if(func3=="000"&&func7=="0100000"){
        op="sub";a006++;}
    else if(func3=="100"&&func7=="0000000"){
        op="xor";a006++;}
    else if(func3=="000"&&func7=="0000001"){
        op="mul";a006++;}
    else if(func3=="100"&&func7=="0000001"){
        op="div";a006++;}
    else if(func3=="110"&&func7=="0000001"){
        op="rem";a006++;}
    string rs2=binary.substr(7,5);
    string rs1=binary.substr(12,5);
    string rd=binary.substr(20,5);
    r2=bin_to_dec(rs2);
    r1=bin_to_dec(rs1);
    r=bin_to_dec(rd);
    imn=INT_MAX;
    cout<<"DECODE: Operation is "<<op<<", "<<"first operand R"<<r2<<", second operand R"<<r1<<", destination register R"<<r<<endl;
    cout<<"DECODE: Read registers R"<<r2<<" = "<<registers[r2]<<", R"<<r1<<" = "<<registers[r1]<<endl;
    return op;
}

string decode_i(string binary,string opcode)
{
    string imm=binary.substr(0,12);
    string rs1=binary.substr(12,5);
    string func3=binary.substr(17,3);
    string rd=binary.substr(20,5);
    string op="";
    if(opcode=="0010011"&&func3=="000"){
        op="addi";a006++;}
    else if(opcode=="0010011"&&func3=="111"){
        op="andi";a006++;}
    else if(opcode=="0010011"&&func3=="110"){
        op="ori";a006++;}
    else if(opcode=="0000011"&&func3=="000"){
        op="lb";a005++;}
    else if(opcode=="0000011"&&func3=="001"){
        op="lh";a005++;}
    else if(opcode=="0000011"&&func3=="010"){
        op="lw";a005++;}
    else if(opcode=="1100111"&&func3=="000"){
        op="jalr";}
    r1=bin_to_dec(rs1);
    r2=-2;
    r=bin_to_dec(rd);
    if(imm[0]=='0')
    {
        imn=bin_to_dec(imm);
    }
    else
    {
        int swapping=0;
        for(int i=11;i>=0;i--)
        {
            if(swapping==1)
            {
                if(imm[i]=='1')
                {
                    imm[i]='0';
                    continue;
                }
                if(imm[i]=='0')
                {
                    imm[i]='1';
                    continue;
                }

            }
            if(imm[i]=='1')
            {
                swapping=1;
            }

        }
       imn=bin_to_dec(imm);
        imn=imn*(-1);
    }
    // if(onebitpredictor==1&&opcode=="1100111"&&func3=="000"){
    //     pred[programCounter/4-1].targetadd = programCounter+imn-4;
    //     pred[programCounter/4-1].bht = 2;
    // }
    cout<<"DECODE: Operation is "<<op<<", "<<"first operand R"<<r1<<", destination register R"<<r<<endl;
    cout<<"DECODE: Read registers R"<<r1<<" = "<<registers[r1]<<endl;
    return op;
}

string decode_u(string binary,string opcode)
{
    string imm=binary.substr(0,20);
    string rd=binary.substr(20,5);
    string op="";
    if(opcode=="0010111")
        op="auipc";
    else if(opcode=="0110111")
        op="lui";

    immp = imm + "000000000000";
    r1=-3;
    r2=-4;
    r=bin_to_dec(rd);
    imn=bin_to_dec(immp);
    cout<<"DECODE: Operation is "<<op<<" destination register R"<<r<<endl;
    return op;
}

string decode_uj(string binary,string opcode)
{
    string imm1=binary.substr(0,1);
    string imm2=binary.substr(1,10);
    string imm3=binary.substr(11,1);
    string imm4=binary.substr(12,8);
    string rd=binary.substr(20,5);
    string imm = imm1+imm4+imm3+imm2+"0";
    string op="";
    if(opcode=="1101111")
        op="jal";
    r=bin_to_dec(rd);
    r1=-5;
    r2=-6;
    if(imm[0]=='0')
    {
        imn=bin_to_dec(imm);
    }
    else
    {
        int swapping=0;
        for(int i=20;i>=0;i--)
        {
            if(swapping==1)
            {
                if(imm[i]=='1')
                {
                    imm[i]='0';
                    continue;
                }
                if(imm[i]=='0')
                {
                    imm[i]='1';
                    continue;
                }

            }
            if(imm[i]=='1')
            {
                swapping=1;
            }
        }
       imn=bin_to_dec(imm);
        imn=imn*(-1);
    }
    // if(onebitpredictor==1){cunt++;
    //     pred[programCounter/4-1].targetadd = programCounter+imn-4;
    //     pred[programCounter/4-1].bht = 2; }
    cout<<"DECODE: Operation is "<<op<<" destination register R"<<r<<endl;
    return op;
}

string decode_s(string binary,string opcode)
{
    string imm=binary.substr(0,7);
    imm+=binary.substr(20,5);
    string rs1=binary.substr(12,5);
    string func3=binary.substr(17,3);
    string rd=binary.substr(7,5);
    string op="";
    if(opcode=="0100011"&&func3=="000"){
        op="sb"; a005++;}
    else if(opcode=="0100011"&&func3=="010"){
        op="sw";a005++;}
    else if(opcode=="0100011"&&func3=="011"){
        op="sd";a005++;}
    else if(opcode=="0100011"&&func3=="100"){
        op="sh"; a005++;}
    r1=bin_to_dec(rs1);
    r2=-7;
    r=bin_to_dec(rd);
    if(imm[0]=='0')
    {
        imn=bin_to_dec(imm);
    }
    else
    {
        int swapping=0;
        for(int i=11;i>=0;i--)
        {
            if(swapping==1)
            {
                if(imm[i]=='1')
                {
                    imm[i]='0';
                    continue;
                }
                if(imm[i]=='0')
                {
                    imm[i]='1';
                    continue;
                }

            }
            if(imm[i]=='1')
            {
                swapping=1;
            }

        }
       imn=bin_to_dec(imm);
        imn=imn*(-1);
    }
    cout<<"DECODE: Operation is "<<op<<", "<<"base address is in R"<<r1<<", second operand R"<<r<<endl;
    cout<<"DECODE: Read registers R"<<r<<" = "<<registers[r]<<", R"<<r1<<" = "<<registers[r1]<<endl;
    return op;
}
void flush_function(instr x)
{
        x.instruction="";
        x.reg1=-1;
        x.reg2=-1;
        x.final_reg=-1;
        x.stage=0;
        x.value=0;
        x.value_imm=0;
        x.exec=false;
        x.stalled=false;
        x.reg1value=0;
        x.reg2value=0;
        x.final_regvalue=0;
        
}

instr execute_sb(instr x)
{
    if(onebitpredictor==1){beqs++;}

    if(x.operation=="beq"){
        if(x.reg1value==x.reg2value){
            // if(pred[programCounter/4-1].bht == 0&&onebitpredictor==1){pred[programCounter/4-1].bht = 1; miss++;
            //     falg=1;
            // }
            // else if(pred[programCounter/4-1].bht == 1&&onebitpredictor==1){hit++;}
            programCounter=x.progcount+x.value_imm;
        }
        // else if(pred[programCounter/4-1].bht == 1&&onebitpredictor==1){pred[programCounter/4-1].bht = 0; miss++;falg=1;}
        // else if(pred[programCounter/4-1].bht == 0&&onebitpredictor==1){hit++;}
    }

    else if(x.operation=="bne"){
        if(x.reg1value!=x.reg2value){
            // if(pred[programCounter/4-1].bht == 0&&onebitpredictor==1){pred[programCounter/4-1].bht = 1;miss++; falg=1;}
            // else if(pred[programCounter/4-1].bht == 1&&onebitpredictor==1){hit++;}
            programCounter=x.progcount+x.value_imm;
        }
        // else if(pred[programCounter/4-1].bht == 1&&onebitpredictor==1){pred[programCounter/4-1].bht = 0; miss++; falg=1;}
        // else if(pred[programCounter/4-1].bht == 0&&onebitpredictor==1){hit++;}
    }

    else if(x.operation=="blt"){
        if(x.reg1value<x.reg2value){
            // if(pred[programCounter/4-1].bht == 0&&onebitpredictor==1){pred[programCounter/4-1].bht = 1; miss++; falg=1;}
            // else if(pred[progr/amCounter/4-1].bht == 1&&onebitpredictor==1){hit++;}
            programCounter=x.progcount+x.value_imm;
        }
        // else if(pred[programCounter/4-1].bht == 1&&onebitpredictor==1){pred[programCounter/4-1].bht = 0; miss++; falg=1;}
        // else if(pred[programCounter/4-1].bht == 0&&onebitpredictor==1){hit++;}
    }

    else if(x.operation=="bge"){
        if(x.reg1value>=x.reg2value){
            // if(pred[programCounter/4-1].bht == 0&&onebitpredictor==1){pred[programCounter/4-1].bht = 1; miss++; falg=1;}
            // else if(pred[programCounter/4-1].bht == 1&&onebitpredictor==1){hit++;}
            programCounter=x.progcount+x.value_imm;
        }
        // else if(pred[programCounter/4-1].bht == 1&&onebitpredictor==1){pred[programCounter/4-1].bht = 0; miss++; falg=1;}
        // else if(pred[programCounter/4-1].bht == 0&&onebitpredictor==1){hit++;}
    }
    cout<<"EXECUTE: "<<x.operation<<" "<<x.reg2value<<" and "<<x.reg1value<<endl;
    return x;
}

instr execute_uj(instr x)
{
    if(onebitpredictor==1){jali++;}
    x.yy=x.progcount+4;
    programCounter=x.progcount+x.value_imm;
    cout<<"EXECUTE: "<<x.operation<<" "<<x.final_regvalue<<endl;
    return x;
}

instr execute_s(instr x)
{
        x.val= x.value_imm+x.reg1value;
    cout<<"EXECUTE: "<<x.operation<<" "<<x.final_regvalue<<" in "<<x.val<<endl;
    return x;
}

instr execute_u(instr x)
{
    if(x.operation=="auipc"){
        x.yy= programCounter+x.value_imm;
    }
    else if(x.operation=="lui")
        x.yy=x.value_imm;
  
    cout<<"EXECUTE: "<<x.operation<<" "<<x.final_regvalue<<endl;
    cout<<"Value in temporary register RY="<<x.yy<<endl;
    return x;
}

instr execute_i(instr x)
{
    if(x.operation=="addi")
        x.yy=x.reg1value+x.value_imm;
    else if(x.operation=="andi")
        x.yy=x.reg1value&x.value_imm;
    else if(x.operation=="ori")
        x.yy=x.reg1value|x.value_imm;
    else if(x.operation=="lb"||x.operation=="lh"||x.operation=="lw")
    {
        x.val=x.value_imm+x.reg1value;
    }
    else if(x.operation=="jalr")
    {
        if(onebitpredictor==1){jalsi++;}
        x.yy=x.progcount+4;
        programCounter=x.value_imm+x.reg1value;
    }
    cout<<"EXECUTE: "<<x.operation<<" "<<x.reg1value<<" and "<<x.value_imm<<endl;
    return x;
}

instr execute_r(instr x)
{
    if(x.operation=="add")
        x.yy=x.reg1value+x.reg2value;
    else if(x.operation=="sub")
        x.yy=x.reg1value-x.reg2value;
    else if(x.operation=="xor")
        x.yy=x.reg1value^x.reg2value;
    else if(x.operation=="mul")
        x.yy=x.reg1value*x.reg2value;
    else if(x.operation=="div")
        x.yy=x.reg1value/x.reg2value;
    else if(x.operation=="rem")
        x.yy=x.reg1value%x.reg2value;
    cout<<"EXECUTE: "<<x.operation<<" "<<x.reg2value<<" and "<<x.reg1value<<endl;
    cout<<"Value in temporary register RY= "<<x.yy<<endl;
    return x;
}

void reg_write(int finalv,int yy,int proc)
{
    registers[finalv]=yy;
    cout<<"Value of destination register R "<<finalv<<" in decimal= "<<registers[finalv]<<endl;
    cout<<"Value of destination register R "<<finalv<<" in hexa= "<<dec_to_hex(registers[finalv])<<endl;
    cout<<"Program Counter= "<<proc<<endl;
    return;
}

void write_word(instr as)
{
    string temp =dec_to_hex(as.final_regvalue);
    cout<<"REGVALUE= "<<as.final_regvalue<<"\n";
    string hexa=dec_to_hex(as.val);
    if(as.val>=268435456)
    {
        memory[as.val-initial+3] = temp.substr(2,2);
        memory[as.val-initial+2] = temp.substr(4,2);
        memory[as.val-initial+1] = temp.substr(6,2);
        memory[as.val-initial] = temp.substr(8,2);
        cout<<"Memory at "<<hexa<<"= "<<(memory[as.val-initial+3]+memory[as.val-initial+2]+memory[as.val-initial+1]+memory[as.val-initial])<<endl;
    }
    else
    {
        memory_i[as.val+3] = temp.substr(2,2);
        memory_i[as.val+2] = temp.substr(4,2);
        memory_i[as.val+1] = temp.substr(6,2);
        memory_i[as.val] = temp.substr(8,2);
        cout<<"Memory at "<<hexa<<"= "<<(memory_i[as.val+3]+memory_i[as.val+2]+memory_i[as.val+1]+memory_i[as.val])<<endl;
    }
    return;
}

void write_byte(instr as)
{
    string hexa=dec_to_hex(as.val);
    if(as.val>=268435456)
    {
        memory[as.val-initial] = dec_to_hex(as.final_regvalue).substr(8,2);
        cout<<"Memory at "<<hexa<<"= "<<(memory[as.val-initial])<<endl;
    }
    else
    {
        memory_i[as.val]=dec_to_hex(as.final_regvalue).substr(8,2);
        cout<<"Memory at "<<hexa<<"= "<<(memory_i[as.val])<<endl;
    }
    
    return;
}

void write_half(instr as)
{
    string hexa=dec_to_hex(as.val);
    if(as.val>=268435456)
    {
        memory[as.val-initial] = dec_to_hex(as.final_regvalue).substr(8,2);
        memory[as.val-initial+1] = dec_to_hex(as.final_regvalue).substr(6,2);
        cout<<"Memory at "<<hexa<<"= "<<(memory[as.val-initial+1]+memory[as.val-initial])<<endl;
    }
    else
    {
        memory_i[as.val] = dec_to_hex(as.final_regvalue).substr(8,2);
        memory_i[as.val+1] = dec_to_hex(as.final_regvalue).substr(6,2);
        cout<<"Memory at "<<hexa<<"= "<<(memory_i[as.val+1]+memory_i[as.val])<<endl;
    }
    return;
}

instr mem_access(instr as)
{
    if(as.operation=="sb"){write_byte(as);}
    else if(as.operation=="sw"){
        write_word(as);}
    else if(as.operation== "sh"){write_half(as);}
    else if(as.operation=="lb")
    {
        if(as.val>=268435456)
            as.yy=hex_to_dec(memory[as.val-initial]);
        else
            as.yy=hex_to_dec(memory_i[as.val]);
        cout<<"Memory accessed by load\n";
        cout<<"Value stored in temporary register used by load\n"<<as.yy<<"\n";
    }
    else if(as.operation=="lh")
    {
        string str;
        if(as.val>=268435456)
            str=memory[as.val-initial+1]+memory[as.val-initial];
        else
            str=memory_i[as.val+1]+memory_i[as.val];
        as.yy=hex_to_dec(str);
        cout<<"Memory accessed by load\n";
        cout<<"Value stored in temporary register used by load\n"<<as.yy<<"\n";
    }
    else if(as.operation=="lw")
    {
        string str;
        if(as.val>=268435456)
            str=memory[as.val-initial+3]+memory[as.val-initial+2]+memory[as.val-initial+1]+memory[as.val-initial];
        else
            str=memory_i[as.val+3]+memory_i[as.val+2]+memory_i[as.val+1]+memory_i[as.val];
        as.yy=hex_to_dec(str);
        cout<<"Memory accessed by load\n";
        cout<<"Value stored in temporary register used by load\n"<<as.yy<<"\n";
    }
    return as;
}

instr fetch(instr x)
{
    if(x.stage==0&&x.exec==false)
    {
        binary=instr_memory[programCounter/4];
        x.progcount=programCounter;
        cout<<"FETCH:Fetch instruction 0x"<<binary<<" from address 0x"<<dec_to_hex(programCounter)<<endl;
        programCounter=programCounter+4;
    }
    else
    {
        subtr=true;
        binary=instr_memory[x.progcount/4];
        cout<<"FETCH:Fetch instruction 0x"<<binary<<" from address 0x"<<dec_to_hex(x.progcount)<<endl;
    }


    x.instruction=binary;
    x.stage=1;
    return x;
}

instr decode(instr x)
{
    int flags=0;
    string op;
    string opcode=x.instruction.substr(25,7);
    cout<<"Decode"<<endl;
    if(opcode=="0110011")
    {
        op=decode_r(x.instruction);
    }
    else if(opcode=="0000011"||opcode=="0010011"||opcode=="1100111")
    {
        op=decode_i(x.instruction,opcode);
    }
    else if(opcode=="0100011")
    {
        op = decode_s(x.instruction, opcode);
    }
    else if(opcode=="0010111"||opcode=="0110111")
    {
        op = decode_u(x.instruction, opcode);
    }
    else if(opcode=="1101111")
    {
        op=decode_uj(x.instruction,opcode);
    }
    else if(opcode=="1100011")
    {
        op=decode_sb(x.instruction,opcode);
    }
    int flagz=0;
    x.reg1=r1;
    x.reg2=r2;
    x.final_reg=r;
    x.operation=op;
    if(x.reg1>=0)
        x.reg1value=registers[x.reg1];
    if(x.reg2>=0)
        x.reg2value=registers[x.reg2];
    if(x.final_reg>=0)
        x.final_regvalue=registers[x.final_reg];
    if(choiced==0)
    {
        for(int il=0;il<ilo;il++)
        {
            if(x.reg1==pipelining[il].final_reg||x.reg2==pipelining[il].final_reg)
            {
                if(pipelining[il].operation!="sb"&&pipelining[il].operation!="sw"&&pipelining[il].operation!="sh"&&pipelining[il].stage<=4)
                    {
                            flagz=1;
                            x.stage=1;
                            stalld++;
                            x.stalled=true;
                            x.didstall=1;
                            cout<<"STALL\n";
                            break;
                    }
                    
                    /*if(pipelining[il].stage<=3)
                    {
                        flagz=1;
                        x.stage=1;
                        stall++;
                        x.stalled=true;
                        break;
                    }*/

            }
            if(x.operation=="sb"||x.operation=="sw"||x.operation=="sh")
            {
                if(x.final_reg==pipelining[il].final_reg)
                {
                    if(pipelining[il].stage<=4)
                    {
                        flagz=1;
                        x.stage=1;
                        stalld++;
                        x.stalled=true;
                        x.didstall=1;
                        break;
                    }
                }
            }
        }
    }
    
    if(onebitpredictor==1&&opcode=="1100011"){ a002++;
    pred[x.progcount/4].targetadd = x.progcount+imn;}
    
    if(onebitpredictor==1&&opcode=="1100111"&&x.operation=="jalr"){ a003++;
        pred[x.progcount/4].targetadd = x.progcount+imn;
        pred[x.progcount/4].bht = 2;
    }
    if(onebitpredictor==1&&opcode=="1101111"){ a004++;
        pred[x.progcount/4].targetadd = x.progcount+imn;
        pred[x.progcount/4].bht = 2;
    }

    if(flagz==0)
    {
        x.stalled=false;
        x.stage=2;
    }
    if(x.operation=="sb"||x.operation=="sw"||x.operation=="sh")
    {
        x.yy=x.final_regvalue;
    }
    x.value_imm=imn;
    return x;
}


instr execute(instr x)
{
    string opcode=x.instruction.substr(25,7);
    if(opcode=="0110011")
    {
        x=execute_r(x);
    }
    else if(opcode=="0000011"||opcode=="0010011"||opcode=="1100111")
    {
        x=execute_i(x);
    }
    else if(opcode=="0100011")
    {
        x=execute_s(x);
    }
    else if(opcode=="0010111"||opcode=="0110111")
    {
        x=execute_u(x);
    }
    else if(opcode=="1101111")
    {
        x=execute_uj(x);
    }
    else if(opcode=="1100011")
    {
        x=execute_sb(x);
    }
    return x;
}

instr mem_write(instr x)
{
    string opcode=x.instruction.substr(25,7);
    if(opcode=="0100011"||opcode=="0000011")
    {
        x=mem_access(x);
    }
    else
    {
        cout<<"MEMORY:No memory operation"<<endl;
    }
    x.stage=4;
    return x;
}

instr reg_update(instr x)
{
    string opcode=x.instruction.substr(25,7);
if(opcode=="0110011"||opcode=="0000011"||opcode=="0010011"||opcode=="1100111"||opcode=="0010111"||opcode=="0110111"||opcode=="1101111")
    {
        reg_write(x.final_reg,x.yy,x.progcount);
    }
    else
    {
        cout<<"No register update operation\n";
    }
    x.stage=5;
    return x;
}

int main()
{
    for(i=0;i<31;i++)
    {
        registers[i]=0;
    }
    registers[2]=234568;
    registers[3]=268435456;
    ifstream fin;
    string line;
    for(int ia=0; ia<10000; ia++){pred[ia].bht = 1; pred[ia].targetadd = -1;}
    string word="",pc="",tc="";
    fin.open("example.txt");
    cout<<"Do you want to enable pipelining? Enter 1 for Yes 0 for No\n";
    cin>>choicep;
    if(choicep==1)
    {
        cout<<"Do you want to enable data forwarding? Enter 1 for Yes 0 for No\n";
        cin>>choiced;
        cout<<"Do you want to enable One bit predictor? Enter 1 for Yes 0 for No\n";
        cin>>onebitpredictor;
    }
    cout<<"Do you want to print values in the register file at the end of each cycle? Enter 1 for Yes 0 for No\n";
    cin>>choicer;
    cout<<"Do you want to print information in the pipeline registers at the end of each cycle? Enter 1 for Yes 0 for No\n";
    cin>>choicepipe;
    cout<<"Do you want to print information in the pipeline registers at the end of each cycle for a specific instruction? Enter 1 for Yes 0 for No\n";
    cin>>choicepipe2;
    if(choicepipe2==1)
    {
        cout<<"Enter instruction number.\n";
        cin>>innum;
    }
    int aa1=0;
    int aa2=0;
    while(fin.eof()==false)
    {
        getline(fin,line);
        int tpo=0;
        for(int i1=0; i1<line.size()-2; i1++){
            if(line[i1]=='E'&&line[i1+1]=='N'&&line[i1+2]=='D'){tpo=1;}
        }
        if(tpo>0){ break;}
        
        word="";
        for(int i1=0; i1<line.length(); i1++)
        {
                if(line[i1]==' ')
                {
                    break;
                }
                else
                {
                    pc = pc + line[i1];
                }
        }
        string tc="";
        for(int i=0; i<pc.length(); i++)
        {
            if(pc[i]=='x')
            {
                for(int j=i+1; j<pc.length(); j++)
                {
                    tc=tc+pc[j];
                }
            }
        }
        pc = tc; int cnt=1;
        programCounter=0;
        programCounter=hex_to_dec(pc);
        kac=programCounter;
        
        for(int i=0;i<line.length();i++)
        {
            if(line[i]==' ')
            {
                int cc=0;
                for(int j=i+3;j<line.length();j++)
                {
                    if(cc==9) break;
                    word=word+line[j];
                    cc=cc+1;
                }
                if(cc==9) break;
            }
        }
        //decode word
        string binary="";
        for(int i=0;i<8;i++)
        {
            string partial;
            partial=word[i];
            binary=binary+lookup(partial);
        }
        memory_i[aa1]=binary.substr(8,2);
        memory_i[aa1+1]=binary.substr(6,2);
        memory_i[aa1+1]=binary.substr(4,2);
        memory_i[aa1+3]=binary.substr(2,2);

        instr_memory[aa2]=binary;
        aa1+=4;
        aa2+=1;
    }
    
    vector <string> dotdata; vector <string> address; vector <int> minedata;vector <int> addressint; int mynumber=0;
    while(fin.eof()==false){
        string as;
        getline(fin,line);
        dotdata.push_back(line); string addd=""; int opo; string mydata="";
        for(int i=0; i<line.size(); i++){
            if(line[i]==' '){opo = i;break;}
            addd = addd+line[i];
        }
        for(int i=opo+1; i<line.size(); i++){
            mydata = mydata+line[i];
        }
        
        
        address.push_back(addd);
        addressint.push_back(hex_to_dec_ad(addd));
        minedata.push_back(hex_to_dec_ad(mydata));
        
    }
    
    fin.close();

    if(dotdata.size()>1){
    for(int ii=0; ii<dotdata.size()-1; ii++){
        for(int i=0; i<addressint[ii+1]-addressint[ii]; i++){
            memory[addressint[ii]+i-initial] = dec_to_hex(minedata[ii]).substr(8-2*i,2);}
    }
        for(int i=0; i<4; i++){
        memory[addressint[dotdata.size()-1]+i-initial] = dec_to_hex(minedata[dotdata.size()-1]).substr(8-2*i,2);}
    }
    else if(dotdata.size()==1){
        for(int i=0; i<4; i++){ cout<<addressint[dotdata.size()-1]+i-initial<<endl;
        memory[addressint[dotdata.size()-1]+i-initial] = dec_to_hex(minedata[dotdata.size()-1]).substr(8-2*i,2);}
    }
    
    //INSTRUCTION EXECUTION PART
    
    programCounter=0;
    for(int i=0;i<10000;i++)
    {
        pipelining[i].instruction="";
        pipelining[i].reg1=-1;
        pipelining[i].reg2=-1;
        pipelining[i].final_reg=-1;
        pipelining[i].stage=0;
        pipelining[i].value=0;
        pipelining[i].value_imm=0;
        pipelining[i].exec=false;
        pipelining[i].stalled=false;
        pipelining[i].reg1value=0;
        pipelining[i].reg2value=0;
        pipelining[i].final_regvalue=0;
        pipelining[i].didstall=0;
    }
    int index=0;
    int indexc=0;
    int flagy=0;
    programCounter=0;
    ilo=0;
    if(choicep==1)
    {
        while(1)
        {
            cout<<"Cycles : "<<cycles+1<<"Index= "<<index<<"\n";
            countpp=0;
            for(ilo=0;ilo<index+1;ilo++)
            {
                if(pipelining[ilo].stage==5)
                {
                    countpp++;
                    continue;
                }
                if(pipelining[ilo].stage==0)
                {
                    pipelining[ilo]=fetch(pipelining[ilo]);
                    pipelining[ilo].exec=true;
                    
                    if((ilo>=1)&&(pipelining[ilo-1].stage==1))
                    {
                        pipelining[ilo].stage=0;
                    }
                    if(pipelining[ilo].stage==1)
                        indexc++;
                    
                    if(onebitpredictor==1&&pred[pipelining[ilo].progcount/4].targetadd!=-1){
                        if(pred[pipelining[ilo].progcount/4].bht==1){
                            pipelining[ilo+1].progcount = pred[pipelining[ilo].progcount/4].targetadd;
                        }
                        else if(pred[pipelining[ilo].progcount/4].bht==0){
                            pipelining[ilo+1].progcount = pipelining[ilo].progcount+4;
                        }
                    }
                    continue;
                }
                
                if(pipelining[ilo].stage==1)
                {
                    pipelining[ilo]=decode(pipelining[ilo]);
                    
                    // if(onebitpredictor==1&&(pipelining[ilo].operation=="beq"||pipelining[ilo].operation=="bge"||pipelining[ilo].operation=="blt"||pipelining[ilo].operation=="bne"||pipelining[ilo].operation=="jal"||pipelining[ilo].operation=="jalr"))
                    // {
                    //     if(pred[pipelining[ilo].progcount/4].bht==1||pred[pipelining[ilo].progcount/4].bht==2){
                    //         pipelining[ilo+1].progcount = pred[pipelining[ilo].progcount/4].targetadd-4;
                            
                    //     }
                    //     // else{
                    //     //     pipelining[ilo+1].progcount = pipelining[ilo].progcount+4;
                    //     // }
                       
                    // }
            
 if(pipelining[ilo].operation=="beq"||pipelining[ilo].operation=="bge"||pipelining[ilo].operation=="blt"||pipelining[ilo].operation=="bne"||pipelining[ilo].operation=="jal"||pipelining[ilo].operation=="jalr")
                    {
                        if(pipelining[ilo].stalled==false)
                        {
                            cout<<"STALL\n";
                            indexc=ilo;
                            stallc++;
                            break;
                        }
                        else
                        {
                            cout<<"STALL\n";
                            indexc=ilo;
                            break;
                        }
                    }
                    continue;
                }
                
                if(pipelining[ilo].stage==2)
                {
                    string opera;
                    bool change1=false,change2=false,changef=false;

                    //onebitpred
                    // if(onebitpredictor==1&&falg==1){
                    //     flush_function(ilo, pipelining[ilo]);
                    //     flush_function(ilo, pipelining[ilo]);
                    //    falg=0;
                    // }

                    //DATA FORWARDING
                
                    if(pipelining[ilo].reg1==pipelining[ilo-1].final_reg&&choiced==1)
                    {
            if(pipelining[ilo-1].operation!="lb"&&pipelining[ilo-1].operation!="lw"&&pipelining[ilo-1].operation!="lh")
                        {
                            pipelining[ilo].reg1value=pipelining[ilo-1].yy;
                            change1=true;
                        }
                        else if((pipelining[ilo-1].operation=="lb"||pipelining[ilo-1].operation=="lw"||pipelining[ilo-1].operation!="lh")&&pipelining[ilo-1].stage>=4)
                        {
                            pipelining[ilo].reg1value=pipelining[ilo-1].yy;
                            change1=true;
                        }
                        else
                        {
                            pipelining[ilo].didstall=1;
                            stalld++;
                            continue;
                        }
                    }
                    if(pipelining[ilo].reg2==pipelining[ilo-1].final_reg&&choiced==1)
                    {
            if(pipelining[ilo-1].operation!="lb"&&pipelining[ilo-1].operation!="lw"&&pipelining[ilo-1].operation!="lh")
                        {
                            pipelining[ilo].reg2value=pipelining[ilo-1].yy;
                            change2=true;
                        }
                        else if((pipelining[ilo-1].operation=="lb"||pipelining[ilo-1].operation=="lw"||pipelining[ilo-1].operation!="lh")&&pipelining[ilo-1].stage>=4)
                        {
                            pipelining[ilo].reg2value=pipelining[ilo-1].yy;
                            change2=true;
                        }
                        else
                        {
                            pipelining[ilo].didstall=1;
                            stalld++;
                            continue;
                        }
                    }
       if((pipelining[ilo].operation=="sb"||pipelining[ilo].operation=="sw"||pipelining[ilo].operation=="sh")&&choiced==1&&pipelining[ilo].final_reg==pipelining[ilo-1].final_reg)
                    {
            if(pipelining[ilo-1].operation!="lb"&&pipelining[ilo-1].operation!="lw"&&pipelining[ilo-1].operation!="lh")             {
                            pipelining[ilo].final_regvalue=pipelining[ilo-1].yy;
                            changef=true;
                          }
                        else
                        {
                            pipelining[ilo].didstall=1;
                            stalld++;
                            continue;
                        }
                    }
                    
                    if(change1==false)
                    {
                        pipelining[ilo].reg1value=registers[pipelining[ilo].reg1];
                    }
                    if(change2==false)
                    {
                        pipelining[ilo].reg2value=registers[pipelining[ilo].reg2];
                    }
                    if(changef==false)
                    {
                        pipelining[ilo].final_regvalue=registers[pipelining[ilo].final_reg];
                    }
                    
                    pipelining[ilo]=execute(pipelining[ilo]);
                    pipelining[ilo].value=registers[pipelining[ilo].final_reg];
                    pipelining[ilo].stage=3;
                    
                    if(onebitpredictor==1&&(pipelining[ilo].operation=="beq"||pipelining[ilo].operation=="bge"||pipelining[ilo].operation=="blt"||pipelining[ilo].operation=="bne"/*||pipelining[ilo].operation=="jal"||pipelining[ilo].operation=="jalr"*/)){
                       //aman.push_back(pipelining[ilo+1].progcount); aman1.push_back(programCounter);
                       //cout<<pipelining[ilo+1].progcount<<" f  "<<programCounter<<endl;
                        if(pipelining[ilo+1].progcount!=programCounter/*&&pred[pipelining[ilo].progcount/4].bht!=2*/){ ad1++;
                            // there is a missprediction by onebit predictor
                            //mc = true;
                            miss++;
                            if(pred[pipelining[ilo].progcount/4].bht==1){pred[pipelining[ilo].progcount/4].bht=0;}
                            else if(pred[pipelining[ilo].progcount/4].bht==0){pred[pipelining[ilo].progcount/4].bht=1;}
                            flush_function(pipelining[ilo+1]);
                            flush_function(pipelining[ilo+2]);
                            pipelining[ilo+1].progcount = programCounter;
                            pipelining[ilo+1].stage = 0;
                            pipelining[ilo+1].exec=false;
                            pipelining[ilo+2].stage = 0;
                            pipelining[ilo+2].exec=false;
                            a001 = a001+2;
                            // indexc=ilo-1;
                            //index++;
                        }
                        else if(pipelining[ilo+1].progcount==programCounter&&pred[pipelining[ilo].progcount/4].bht!=2){hit++;}
                        else{
                            if(programCounter>kac){  indexc=ilo;
                            break;}
                        }
                        indexc=ilo+1;
                            
                        
                    }
else if(pipelining[ilo].operation=="beq"||pipelining[ilo].operation=="bge"||pipelining[ilo].operation=="blt"||pipelining[ilo].operation=="bne"||pipelining[ilo].operation=="jal"||pipelining[ilo].operation=="jalr")
                    {
                        stallc++;
                        if(programCounter<=kac)
                            indexc=ilo+1;
                        else
                            indexc=ilo;
                        pipelining[ilo+2].stage=0;
                        pipelining[ilo+2].exec=false;
                        break;
                    }
                    continue;
                }
                
                if(pipelining[ilo].stage==3)
                {
                   //DATA FORWARDING
                    if((pipelining[ilo].operation=="sb"||pipelining[ilo].operation=="sw"||pipelining[ilo].operation=="sh")&&choiced==1)
                       {
                           if((pipelining[ilo-1].operation=="lb"||pipelining[ilo-1].operation=="lw"||pipelining[ilo-1].operation=="lh")&&pipelining[ilo-1].stage>=4)
                           {
                               pipelining[ilo].final_regvalue=pipelining[ilo-1].yy;
                           }
                       }

                       
                    pipelining[ilo]=mem_write(pipelining[ilo]);
                    continue;
                }

                if(pipelining[ilo].stage==4)
                {
                    if(pipelining[ilo].operation=="lb"||pipelining[ilo].operation=="lh"||pipelining[ilo].operation=="lw"||pipelining[ilo].operation=="sw"||pipelining[ilo].operation=="sh"|| pipelining[ilo].operation=="sb")
                    {
                        nls++;
                    }
                    else if(pipelining[ilo].operation=="beq"||pipelining[ilo].operation=="bge"||pipelining[ilo].operation=="blt"||pipelining[ilo].operation=="bne"||pipelining[ilo].operation=="jal"||pipelining[ilo].operation=="jalr")
                    {
                        nci++;
                    }
                    else
                    {
                        nai++;
                    }
                    dh=dh+pipelining[ilo].didstall;
                    pipelining[ilo]=reg_update(pipelining[ilo]);
                    continue;
                }
                
            }
            cout<<"\n";
            if(choicepipe==1)
            {
                for(i=0;i<index+1;i++)
                {
                    if(pipelining[i].stage<=4)
                    {
                        cout<<"Cycle No: "<<cycles+1<<"\n";
                        cout<<"Instruction= "<<pipelining[i].instruction<<"\n";
                        cout<<"Register 1= "<<pipelining[i].reg1<<"\n";
                        cout<<"Register 2= "<<pipelining[i].reg2<<"\n";
                        cout<<"Final register= "<<pipelining[i].final_reg<<"\n";
                        cout<<"Register 1 value= "<<pipelining[i].reg1value<<"\n";
                        cout<<"Register 2 value= "<<pipelining[i].reg2value<<"\n";
                        cout<<"Final Register value= "<<pipelining[i].final_regvalue<<"\n";
                        cout<<"Immediate value= "<<pipelining[i].value_imm<<"\n";
                        cout<<"Temporary register value= "<<pipelining[i].yy<<"\n";
                        cout<<"Program counter value of instruction= "<<pipelining[i].progcount<<"\n";
                    }
                }
            }
            
            if(choicepipe2==1)
            {
                cout<<"Cycle No: "<<cycles+1<<"\n";
                cout<<"Instruction= "<<pipelining[innum-1].instruction<<"\n";
                cout<<"Register 1= "<<pipelining[innum-1].reg1<<"\n";
                cout<<"Register 2= "<<pipelining[innum-1].reg2<<"\n";
                cout<<"Final register= "<<pipelining[innum-1].final_reg<<"\n";
                cout<<"Register 1 value= "<<pipelining[innum-1].reg1value<<"\n";
                cout<<"Register 2 value= "<<pipelining[innum-1].reg2value<<"\n";
                cout<<"Final Register value= "<<pipelining[innum-1].final_regvalue<<"\n";
                cout<<"Immediate value= "<<pipelining[innum-1].value_imm<<"\n";
                cout<<"Temporary register value= "<<pipelining[innum-1].yy<<"\n";
                cout<<"Program counter value of instruction= "<<pipelining[innum-1].progcount<<"\n";
            }
            if(choicer==1)
            {
                for(i=0;i<32;i++)
                {
                    if(i==0)
                    {
                        cout<<"Register "<<i<<" = "<<0<<endl;
                    }
                    else
                        cout<<"Register "<<i<<" = "<<registers[i]<<endl;
                }
            }
            cout<<"\n";
            
            index=indexc;
            cycles++;
            int temps = ilo;
            if(countpp ==index+1)
            {
                break;
            }
        }
    }
    else
    {
        instr instruction_p2[10000];
        for(int i=0;i<10000;i++)
        {
            instruction_p2[i].instruction="";
            instruction_p2[i].reg1=-1;
            instruction_p2[i].reg2=-1;
            instruction_p2[i].final_reg=-1;
            instruction_p2[i].stage=0;
            instruction_p2[i].value=0;
            instruction_p2[i].value_imm=0;
            instruction_p2[i].exec=false;
            instruction_p2[i].stalled=false;
            instruction_p2[i].reg1value=0;
            instruction_p2[i].reg2value=0;
            instruction_p2[i].final_regvalue=0;
        }
        while(programCounter<=kac)
        {
            instruction_p2[ilo]=fetch(instruction_p2[ilo]);
            cout<<"\n";
            if(choicer==1)
            {
                for(i=0;i<32;i++)
                {
                    if(i==0)
                    {
                        cout<<"Register "<<i<<" = "<<0<<endl;
                    }
                    else
                        cout<<"Register "<<i<<" = "<<registers[i]<<endl;
                }
            }

            instruction_p2[ilo]=decode(instruction_p2[ilo]);
            
            cout<<"\n";
            
            if(choicer==1)
            {
                for(i=0;i<32;i++)
                {
                    if(i==0)
                    {
                        cout<<"Register "<<i<<" = "<<0<<endl;
                    }
                    else
                        cout<<"Register "<<i<<" = "<<registers[i]<<endl;
                }
            }
            
            instruction_p2[ilo]=execute(instruction_p2[ilo]);
            cout<<"\n";
            
            if(choicer==1)
            {
                for(i=0;i<32;i++)
                {
                    if(i==0)
                    {
                        cout<<"Register "<<i<<" = "<<0<<endl;
                    }
                    else
                        cout<<"Register "<<i<<" = "<<registers[i]<<endl;
                }
            }

            instruction_p2[ilo]=mem_write(instruction_p2[ilo]);
            cout<<"\n";
          
            if(choicer==1)
            {
                for(i=0;i<32;i++)
                {
                    if(i==0)
                    {
                        cout<<"Register "<<i<<" = "<<0<<endl;
                    }
                    else
                        cout<<"Register "<<i<<" = "<<registers[i]<<endl;
                }
            }
            instruction_p2[ilo]=reg_update(instruction_p2[ilo]);
            cout<<"\n";
            
            
            if(choicer==1)
            {
                for(i=0;i<32;i++)
                {
                    if(i==0)
                    {
                        cout<<"Register "<<i<<" = "<<0<<endl;
                    }
                    else
                        cout<<"Register "<<i<<" = "<<registers[i]<<endl;
                }
            }

            if(instruction_p2[ilo].operation=="lb"||instruction_p2[ilo].operation=="lh"||instruction_p2[ilo].operation=="lw"||instruction_p2[ilo].operation=="sw"||instruction_p2[ilo].operation=="sh"|| instruction_p2[ilo].operation=="sb")
            {
                nls++;
            }
            else if(instruction_p2[ilo].operation=="beq"||instruction_p2[ilo].operation=="bge"||instruction_p2[ilo].operation=="blt"||instruction_p2[ilo].operation=="jal"||instruction_p2[ilo].operation=="jalr"|| instruction_p2[ilo].operation=="bne")
            {
                nci++;
            }
            else
            {
                nai++;
            }
            ilo++;
            cycles=cycles+5;
            countpp++;
        }
    }
    cout<<"No: of cycles= "<<cycles-1<<"\n";
    cout<<"No: of stalls= "<<stallc+stalld<<"\n";
    cout<<"No: of instructions executed= "<<countpp<<"\n";
    cout<<"CPI= "<<(cycles-1)/(double)countpp<<endl;
    cout<<"No. of ALU instructions= "<<nai<<endl;
    cout<<"No: of Data Transfer ie Load and Store instruction executed= "<<nls<<endl;
    cout<<"No: of control instructions= "<<nci<<"\n";
    cout<<"No: of data hazards= "<<dh<<"\n";
    cout<<"No: of misprediction by one bit predictor= "<<miss<<"\n";
    cout<<"No: of Hit(right prediction) by one bit predictor= "<<hit<<"\n";
    cout<<"No: of Stalling due to Control Hazard= "<<a001+stallc<<"\n";
    cout<<"No: of Control Hazards= "<<a002+a003+a004<<"\n";
    cout<<"No: of Stalling due to data Hazards= "<<stalld<<"\n";

    
    for(i=0;i<32;i++)
    {
        if(i==0)
        {
            cout<<"Register "<<i<<" = "<<0<<endl;
        }
        else
            cout<<"Register "<<i<<" = "<<registers[i]<<endl;
    }
    
    ofstream fout;
    fout.open("output.txt");
    for(int i=0; i<435456; i=i+4){
        fout<<"memory["<<dec_to_hex(initial+i)<<"] = ";
        for(int j=0;j<4; j++)
        {
            fout<<memory[i+j]<<" ";
        }
        fout<<endl;
    }
    if(onebitpredictor==1){
    ofstream fout1;
    fout1.open("onebit_pred.txt");
    fout1<<"-------------- BTB ------------\n";
    fout1<<"  PC  Last_pred  Target_address\n";
    cout<<"\n";
    for(int i=0; i<10000; i++){
        if(pred[i].targetadd!=-1){
        fout1<<" "<<dec_to_hex(i*4)<<" "<<pred[i].bht<<" "<<dec_to_hex(pred[i].targetadd)<<endl;}
       // cout<<"BTB:  PC: "<<dec_to_hex(i*4)<<" Last_pred: "<<pred[i].bht<<" target_address: "<<dec_to_hex(pred[i].targetadd)<<endl;}
    }
    }
   
    
}

