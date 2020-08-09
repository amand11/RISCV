#include <iostream>
#include <fstream>
#include <vector>
#include<string.h>
using namespace std;
int registers[32];
string memory[268435456],immp="",rry="";
vector<string> instr_memory;
vector<string> memory_i;
string binary;
#define initial 268435456
int i=0,r,r1,r2,ry=0,rz=0,clocks=0,imn,ka=0,sy,programCounter=0,kac=0;
long long int val=0;
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
    if(func3=="000"&&func7=="0000000")
        op="add";
    else if(func3=="000"&&func7=="0100000")
        op="sub";
    else if(func3=="100"&&func7=="0000000")
        op="xor";
    else if(func3=="000"&&func7=="0000001")
        op="mul";
    else if(func3=="100"&&func7=="0000001")
        op="div";
    else if(func3=="110"&&func7=="0000001")
        op="rem";
    string rs2=binary.substr(7,5);
    string rs1=binary.substr(12,5);
    string rd=binary.substr(20,5);
    r2=bin_to_dec(rs2);
    r1=bin_to_dec(rs1);
    r=bin_to_dec(rd);
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
    if(opcode=="0010011"&&func3=="000")
        op="addi";
    else if(opcode=="0010011"&&func3=="111")
        op="andi";
    else if(opcode=="0010011"&&func3=="110")
        op="ori";
    else if(opcode=="0000011"&&func3=="000")
        op="lb";
    else if(opcode=="0000011"&&func3=="001")
        op="lh";
    else if(opcode=="0000011"&&func3=="010")
        op="lw";
    else if(opcode=="1100111"&&func3=="000")
        op="jalr";
    r1=bin_to_dec(rs1);
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
    if(opcode=="0100011"&&func3=="000")
        op="sb";
    else if(opcode=="0100011"&&func3=="010")
        op="sw";
    else if(opcode=="0100011"&&func3=="011")
        op="sd";
    else if(opcode=="0100011"&&func3=="100")
        op="sh";
    r1=bin_to_dec(rs1);
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
    cout<<"DECODE: Operation is "<<op<<", "<<"base address is in R"<<r1<<", second operand R"<<r2<<endl;
    cout<<"DECODE: Read registers R"<<r2<<" = "<<registers[r2]<<", R"<<r1<<" = "<<registers[r1]<<endl;
    return op;
}

string execute_sb(string op)
{
    if(op=="beq"){
        if(registers[r1]==registers[r2]){
            programCounter=programCounter+imn-4;
                  }
    }

    else if(op=="bne"){
        if(registers[r1]!=registers[r2]){
            programCounter=programCounter+imn-4;
        }
    }

    else if(op=="blt"){
        if(registers[r1]<registers[r2]){
            programCounter=programCounter+imn-4;
        }
    }

    else if(op=="bge"){
        if(registers[r1]>=registers[r2]){
            programCounter=programCounter+imn-4;
        }
    }
    cout<<"EXECUTE: "<<op<<" "<<registers[r2]<<" and "<<registers[r1]<<endl;
    return "";
}

string execute_uj(string op)
{
    ry=programCounter+4;
    programCounter=programCounter+imn-4;
    cout<<"EXECUTE: "<<op<<" "<<registers[r]<<endl;
    return "";
}

string execute_s(string op)
{
    if(op=="sb"){
        val= imn+registers[r1];}
       
    else if(op=="sw"){
        val= imn+registers[r1];
    }
   
        
    else if(op=="sd"){
       val=imn+registers[r1];}
       

    else if(op=="sh")
    {
        val=imn+registers[r1];
    }
    cout<<"EXECUTE: "<<op<<" "<<registers[r]<<" in "<<registers[r1]<<endl;
    return "";
}

string execute_u(string op)
{
    if(op=="auipc"){
        ry= programCounter+imn;
    }
    else if(op=="lui")
        ry=imn;
  
    cout<<"EXECUTE: "<<op<<" "<<registers[r]<<endl;
    cout<<"Value in temporary register RY="<<ry<<endl;
    return "";
}

string execute_i(string op)
{
    if(op=="addi")
        ry=registers[r1]+imn;
    else if(op=="andi")
        ry=registers[r1]&imn;
    else if(op=="ori")
        ry=registers[r1]|imn;
    else if(op=="lb")
    {
        val=imn+registers[r1];
        if(val>=268435456)
            ry=hex_to_dec(memory[val-initial]);
        else
            ry=hex_to_dec(memory_i[val]);
    }
    else if(op=="lh")
    {
        val=imn+registers[r1];
        string str;
        if(val>=268435456)
            str=memory[val-initial+1]+memory[val-initial];
        else
            str=memory_i[val+1]+memory_i[val];
        ry=hex_to_dec(str);
    }
    else if(op=="lw")
    {
        val=imn+registers[r1];
        cout<<val<<endl;
        string str;
        if(val>=268435456)
            str=memory[val-initial+3]+memory[val-initial+2]+memory[val-initial+1]+memory[val-initial];
        else
            str=memory_i[val+3]+memory_i[val+2]+memory_i[val+1]+memory_i[val];
        ry=hex_to_dec(str);
    }
    else if(op=="jalr")
    {
        ry=programCounter+4;
        programCounter=imn+registers[r1]-4;
    }
    cout<<"EXECUTE: "<<op<<" "<<registers[r1]<<" and "<<imn<<endl;
    return "";
}

string execute_r(string op)
{
    if(op=="add")
        ry=registers[r1]+registers[r2];
    else if(op=="sub")
        ry=registers[r1]-registers[r2];
    else if(op=="xor")
        ry=registers[r1]^registers[r2];
    else if(op=="mul")
        ry=registers[r1]*registers[r2];
    else if(op=="div")
        ry=registers[r1]/registers[r2];
    else if(op=="rem")
        ry=registers[r1]%registers[r2];
    cout<<"EXECUTE: "<<op<<" "<<registers[r2]<<" and "<<registers[r1]<<endl;
    cout<<"Value in temporary register RY= "<<ry<<endl;
    return "";
}

void reg_write()
{
    registers[r]=ry;
    cout<<"Value of destination register R "<<r<<" in decimal= "<<registers[r]<<endl;
    cout<<"Value of destination register R "<<r<<" in hexa= "<<dec_to_hex(registers[r])<<endl;
    return;
}

void write_word()
{
    string temp =dec_to_hex(registers[r]);
    string hexa=dec_to_hex(val);
    if(val>=268435456)
    {
        memory[val-initial+3] = temp.substr(2,2);
        memory[val-initial+2] = temp.substr(4,2);
        memory[val-initial+1] = temp.substr(6,2);
        memory[val-initial] = temp.substr(8,2);
        cout<<"Memory at "<<hexa<<"= "<<(memory[val-initial+3]+memory[val-initial+2]+memory[val-initial+1]+memory[val-initial])<<endl;
    }
    else
    {
        memory_i[val+3] = temp.substr(2,2);
        memory_i[val+2] = temp.substr(4,2);
        memory_i[val+1] = temp.substr(6,2);
        memory_i[val] = temp.substr(8,2);
        cout<<"Memory at "<<hexa<<"= "<<(memory_i[val+3]+memory_i[val+2]+memory_i[val+1]+memory_i[val])<<endl;
    }
    return;
}

void write_byte()
{
    string hexa=dec_to_hex(val);
    if(val>=268435456)
    {
        memory[val-initial] = dec_to_hex(registers[r]).substr(8,2);
        cout<<"Memory at "<<hexa<<"= "<<(memory[val-initial])<<endl;
    }
    else
    {
        memory_i[val]=dec_to_hex(registers[r]).substr(8,2);
        cout<<"Memory at "<<hexa<<"= "<<(memory_i[val])<<endl;
    }
    
    return;
}

void write_half()
{
    string hexa=dec_to_hex(val);
    if(val>=268435456)
    {
        memory[val-initial] = dec_to_hex(registers[r]).substr(8,2);
        memory[val-initial+1] = dec_to_hex(registers[r]).substr(6,2);
        cout<<"Memory at "<<hexa<<"= "<<(memory[val-initial+1]+memory[val-initial])<<endl;
    }
    else
    {
        memory_i[val] = dec_to_hex(registers[r]).substr(8,2);
        memory_i[val+1] = dec_to_hex(registers[r]).substr(6,2);
        cout<<"Memory at "<<hexa<<"= "<<(memory_i[val+1]+memory_i[val])<<endl;
    }
    return;
}

void mem_access(string as)
{
    if(as=="sb"){write_byte();}
    else if(as=="sw"){
        write_word();}
    else if(as == "sh"){write_half();}
    return;
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
    string word="",pc="",tc="";
    fin.open("example.txt");
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
        memory_i.push_back(binary.substr(8,2));
        memory_i.push_back(binary.substr(6,2));
        memory_i.push_back(binary.substr(4,2));
        memory_i.push_back(binary.substr(2,2));
        instr_memory.push_back(binary);
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
    
    for(programCounter=0;programCounter<=kac;programCounter=programCounter+4)
    {
        binary=instr_memory[programCounter/4];
        cout<<"FETCH:Fetch instruction 0x"<<binary<<" from address 0x"<<dec_to_hex(programCounter)<<endl;
        string opcode=binary.substr(25,7);
        cout<<"Decode"<<endl;
        if(opcode=="0110011")
        {
            string op=decode_r(binary);
            execute_r(op);
            cout<<"MEMORY:No memory operation"<<endl;
            reg_write();
        }
        else if(opcode=="0000011"||opcode=="0010011"||opcode=="1100111")
        {
            string op=decode_i(binary,opcode);
            execute_i(op);
            cout<<"MEMORY:No memory operation"<<endl;
            reg_write();
        }
        else if(opcode=="0100011")
        {
            string op = decode_s(binary, opcode);
            execute_s(op);
            mem_access(op);
        }
        else if(opcode=="0010111"||opcode=="0110111"){
            string op = decode_u(binary, opcode);
            execute_u(op);
            reg_write();
        }
        else if(opcode=="1101111")
        {
            string op=decode_uj(binary,opcode);
            execute_uj(op);
            reg_write();
        }
        else if(opcode=="1100011")
        {
            string op=decode_sb(binary,opcode);
            execute_sb(op);
        }
        clocks++;
        cout<<"Clock = "<<clocks<<endl<<endl;
    }

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
        for(int j=0;j<4; j++){
           
            fout<<memory[i+j]<<" ";
        }
        fout<<endl;
    }

}

