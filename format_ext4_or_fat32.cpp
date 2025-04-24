#include <iostream>
#include <cstring>
using namespace std;
void execute_command(string command)
{
    //.size returns the size of a string
    char command_for_system[command.size()];
    char final_command[command.size()];
    //converting string to char
    for(int i=0;i<command.size();i++)
    {
        command_for_system[i]=command[i];
    }
    strcpy(final_command,command_for_system);
    //executing the command
    system(final_command);
}

int main()
{
    cout<<"Welcome to Formater v 1.0"<<endl;
    char answer;
    cout<<"The types of formats that we support are FAT32 for windows and ext4 for linux,also you can unmount a part of a device too"<<endl;
    cout<<"Give the type of format you wish to do, e.g. fat32"<<endl;
    string device;
    string type_of_format;
    while(1)
    {
        cin>> type_of_format;
        //convert a string to lowercase
        for(int i=0;i<type_of_format.size();i++)
        {
            type_of_format[i]=tolower(type_of_format[i]);
        }
        if(type_of_format=="fat32" || type_of_format =="ext4" || type_of_format=="unmount")
        {
            break;
        }
    }
    cout<<"The listed USBs and Drives will be show:"<<endl;
    cout<<"The USBs will look like /dev/sda or sda1 and in general they will look like /dev/sdx"<<endl;
    system("lsblk");
    cin>>device;
    cout<<"If you are sure for your answer type y otherwise n [y/n]"<<endl;
    cin>>answer;
    while (answer!='y' && answer!='Y' && answer!='n' && answer!='N')
    {
        cout<<"warning wrong letter,please type y ir you are sure else type n"<<endl;
    }
    string command;
    if((answer=='y' || answer =='Y') && type_of_format == "fat32")
    {
        command="sudo mkfs.vfat -F 32 /dev/";
        command+=device;
        execute_command(command);
    }
    else if((answer=='y' || answer =='Y') && type_of_format == "ext4")
    {
        command="sudo mkfs.ext4 /dev/";
        command+=device;
        execute_command(command);
    }
    else if((answer=='y' || answer =='Y') && type_of_format == "unmount")
    {
        command="sudo unmount /dev/";
        command+=device;
        execute_command(command);
    }
    return 0;
}