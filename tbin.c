#include <windows.h>
#include "resource.h"

HWND hWnd, Input_hWnd, Output_hWnd;
char input[8192], output[65536];

BOOL CALLBACK DialogProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ToBinary(unsigned char *input, unsigned char *output, int size);
void FromBinary(unsigned char *input, unsigned char *output, int size);

BOOL AppendWindowText(HWND hWnd, char *String)
{
	char *TempString=NULL;
	int TextLength, StringLength;

	TextLength=GetWindowTextLength(hWnd);
	StringLength=strlen(String);

	TempString=(char *)malloc(TextLength+StringLength+1);

	if(TempString==NULL)
		return FALSE;

	GetWindowText(hWnd, TempString, TextLength+1);
	strcat(TempString, String);
	SetWindowText(hWnd, TempString);

	return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	MSG msg;
	int status;

	hWnd=CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DialogProc);
	ShowWindow(hWnd, SW_SHOW);

	while((status=GetMessage(&msg, 0, 0, 0))!=0)
	{
		if(status==-1)
			return -1;

		if(!IsDialogMessage(hWnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}

BOOL CALLBACK DialogProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
			CheckDlgButton(hWndDlg, IDC_BINARY, BST_CHECKED);
			Input_hWnd=GetDlgItem(hWndDlg, IDC_INPUT);
			Output_hWnd=GetDlgItem(hWndDlg, IDC_OUTPUT);
			break;

		case WM_COMMAND:
			switch(wParam)
			{
				case IDC_CONVERT:
					if(IsDlgButtonChecked(hWndDlg, IDC_BINARY))
					{
						GetWindowText(Input_hWnd, input, 8192);
						ToBinary(input, output, strlen(input));
						SetWindowText(Output_hWnd, output);
					}

					if(IsDlgButtonChecked(hWndDlg, IDC_TEXT))
					{
						GetWindowText(Input_hWnd, input, 65536);
						FromBinary(input, output, strlen(input) );
						SetWindowText(Output_hWnd, output);
					}
					break;

				case IDC_CLEAR:
					SetWindowText(Input_hWnd, "\0");
					SetWindowText(Output_hWnd, "\0");
					break;

				case IDC_EXIT:
					PostQuitMessage(0);
					break;
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_CLOSE:
			DestroyWindow(hWndDlg);
			break;

		default:
			return FALSE;
	}

	return TRUE;
}

typedef union
{
	unsigned char byte;
	struct
	{
		unsigned char b0:1;
		unsigned char b1:1;
		unsigned char b2:1;
		unsigned char b3:1;
		unsigned char b4:1;
		unsigned char b5:1;
		unsigned char b6:1;
		unsigned char b7:1;
	} bits;
} byte_u;

void ToBinary(unsigned char *input, unsigned char *output, int size)
{
	int i, j, k;
	byte_u byte;

	byte.byte=0;
	j=k=0;

	for(i=0;i<size;i++)
	{
		byte.byte=input[i];

		if(byte.bits.b0==1)
			output[j++]='1';
		else
			output[j++]='0';

		if(byte.bits.b1==1)
			output[j++]='1';
		else
			output[j++]='0';

		if(byte.bits.b2==1)
			output[j++]='1';
		else
			output[j++]='0';

		if(byte.bits.b3==1)
			output[j++]='1';
		else
			output[j++]='0';

		if(byte.bits.b4==1)
			output[j++]='1';
		else
			output[j++]='0';

		if(byte.bits.b5==1)
			output[j++]='1';
		else
			output[j++]='0';

		if(byte.bits.b6==1)
			output[j++]='1';
		else
			output[j++]='0';

		if(byte.bits.b7==1)
			output[j++]='1';
		else
			output[j++]='0';

		k++;

		if(k==8)
		{
			output[j++]=13;
			output[j++]=10;
			k=0;
		}
		else
			output[j++]=' ';
	}

	output[j++]='\0';
}

void FromBinary(unsigned char *input, unsigned char *output, int size)
{
	int i, j, k;
	byte_u byte;

	byte.byte=0;
	j=k=0;

	for(i=0;i<size;i+=8)
	{
		if(input[i+7]=='1')
			byte.bits.b7=1;
		if(input[i+7]=='0')
			byte.bits.b7=0;

		if(input[i+6]=='1')
			byte.bits.b6=1;
		if(input[i+6]=='0')
			byte.bits.b6=0;

		if(input[i+5]=='1')
			byte.bits.b5=1;
		if(input[i+5]=='0')
			byte.bits.b5=0;

		if(input[i+4]=='1')
			byte.bits.b4=1;
		if(input[i+4]=='0')
			byte.bits.b4=0;

		if(input[i+3]=='1')
			byte.bits.b3=1;
		if(input[i+3]=='0')
			byte.bits.b3=0;

		if(input[i+2]=='1')
			byte.bits.b2=1;
		if(input[i+2]=='0')
			byte.bits.b2=0;

		if(input[i+1]=='1')
			byte.bits.b1=1;
		if(input[i+1]=='0')
			byte.bits.b1=0;

		if(input[i]=='1')
			byte.bits.b0=1;
		if(input[i]=='0')
			byte.bits.b0=0;

		k++;

		if(k==8)
		{
			i+=2;
			k=0;
		}
		else
			i++;

		output[j++]=byte.byte;
	}

	output[j++]='\0';
}
