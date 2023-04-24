// Reservoir.cpp : implementation file
//

#include "stdafx.h"
#include "GassSimulation1.1.h"
#include "Reservoir.h"

extern int i, j, k, reservoirMakedfirst, gridMakedfirst, l, m, o, n, nr, nteta, nx, ny, nz, type, reservoirMaked, gridMaked, firstTime;
extern double poro[100][100][100], permX[100][100][100], permY[100][100][100], permZ[100][100][100];

IMPLEMENT_DYNAMIC(Reservoir, CDialog)

Reservoir::Reservoir(CWnd* pParent /*=NULL*/)
	: CDialog(Reservoir::IDD, pParent)
	, radioPorosity(0)
	, radioPermeability(0)
	, textPorosity(0)
	, textPorofile(_T(""))
	, textpermX(0)
	, textpermY(0)
	, textpermZ(0)
	, textPermfile(_T(""))
{

}

Reservoir::~Reservoir()
{
}

void Reservoir::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIOPOROSITY, radioPorosity);
	DDX_Radio(pDX, IDC_RADIOPERM, radioPermeability);
	DDX_Text(pDX, IDC_EDITPOROSITY, textPorosity);
	DDX_Text(pDX, IDC_EDITPOROFILE, textPorofile);
	DDX_Text(pDX, IDC_EDITpermX, textpermX);
	DDX_Text(pDX, IDC_EDITpermY, textpermY);
	DDX_Text(pDX, IDC_EDITpermZ, textpermZ);
	DDX_Text(pDX, IDC_EDITPERMFILE, textPermfile);
}


BEGIN_MESSAGE_MAP(Reservoir, CDialog)
	ON_BN_CLICKED(IDC_RADIOPOROSITY, &Reservoir::OnBnClickedRadioporosity)
	ON_BN_CLICKED(IDC_RADIOPOROFILE, &Reservoir::OnBnClickedRadioporofile)
	ON_BN_CLICKED(IDC_RADIOPERM, &Reservoir::OnBnClickedRadioperm)
	ON_BN_CLICKED(IDC_RADIOPERMFILE, &Reservoir::OnBnClickedRadiopermfile)
	ON_BN_CLICKED(IDC_BUTTONPORO, &Reservoir::OnBnClickedButtonporo)
	ON_BN_CLICKED(IDC_BUTTONPERM, &Reservoir::OnBnClickedButtonperm)
	ON_BN_CLICKED(IDOK, &Reservoir::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Reservoir::OnBnClickedCancel)
END_MESSAGE_MAP()


// Reservoir message handlers

void Reservoir::OnBnClickedRadioporosity()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDITPOROFILE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITPOROSITY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTONPORO)->EnableWindow(FALSE);
	UpdateData(true);
}

void Reservoir::OnBnClickedRadioporofile()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDITPOROFILE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDITPOROSITY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONPORO)->EnableWindow(TRUE);
	UpdateData(true);
}

void Reservoir::OnBnClickedRadioperm()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDITPERMFILE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITpermX)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDITpermY)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDITpermZ)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTONPERM)->EnableWindow(FALSE);
	UpdateData(true);
}

void Reservoir::OnBnClickedRadiopermfile()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDITPERMFILE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDITpermX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITpermY)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITpermZ)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONPERM)->EnableWindow(TRUE);
	UpdateData(true);
}

void Reservoir::OnBnClickedButtonporo()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CFileDialog file(true);
	if(file.DoModal()==IDOK)
	{
		textPorofile = file.GetPathName();
		UpdateData(false);
	}
}

void Reservoir::OnBnClickedButtonperm()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CFileDialog file(true);
	if(file.DoModal()==IDOK)
	{
		textPermfile =file.GetPathName();
		UpdateData(false);
	}
}

void Reservoir::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	bool ok=true;
	// TODO: Add extra validation here
	if (((radioPorosity == 0 && (textPorosity > 0 && textPorosity < 1)) || (textPorofile != ""))
		&& ((radioPermeability == 0 && textpermX > 0 && textpermY > 0 && textpermZ > 0) || (textPermfile != "")))
	{
		if(radioPorosity == 0)
		{
			for (k=0;k<nz;k++)
			{
				for(j=0;j<ny;j++)
				{
					for(i=0;i<nx;i++)
					{
						poro[i][j][k] = textPorosity;
					}
				}
			}
		}
		else
		{
			FILE *file;
			int count=0;
			char set_str[20];
			if(textPorofile != "" && (file=_wfopen(textPorofile,L"r"))!=false)
			{
				for (k=0;k<nz/2;k++)
				{
					for(j=0;j<ny;j++)
					{
						for(i=0;i<nx;i++)
						{
							o=0;
							set_str[o]=fgetc(file);
							while(set_str[o]!='\t' && set_str[o]!='\n' && set_str[o]!=EOF)
							{
								o++;
								set_str[o]=fgetc(file);
							}
							o++;
							set_str[o]='\0';
							poro[i][j][k]=atof(set_str);
							++count;
							if(set_str[o-1]==EOF)	//if file is smaller than that we want ok set to false
							{
								if(count<nx*ny*nz/2)
								{
									MessageBox(L"The file for poro values is not right", L"Error", MB_OK|MB_ICONERROR);
									ok=false;	//If count is less than nx*ny*nz set to false and break for statments
									break;	//break innest for statement (i)
								}
							}
						}
						if(ok==false)
						{
							break;	//break middle for statement (j)
						}
					}
					if(ok==false)
					{
						break;	//break outes for statement (k)
					}
				}
				fclose(file);
			}
			else
			{
				MessageBox(L"Can not open Poro file",L"Error",MB_OK|MB_ICONERROR);
				ok=false;
			}
		}
		if(ok && radioPermeability == 0)
		{
			for (k=0;k<nz;k++)
			{
				for(j=0;j<ny;j++)
				{
					for(i=0;i<nx;i++)
					{
						permX[i][j][k]=textpermX;
						permY[i][j][k]=textpermY;
						permZ[i][j][k]=textpermZ;
					}
				}
			}
		}
		else if(ok)
		{
			FILE *file;	
			int count=0;
			char set_str[20];
			if(textPorofile != "" && (file=_wfopen(textPorofile,L"r"))!=0)
			{
				for (k=0;k<nz/2;k++)
				{
					for(j=0;j<ny;j++)
					{
						for(i=0;i<nx;i++)
						{
							m=0;
							while(m!=3)//for (1)permX , (2)permY and (3)permZ count 3 times and get string value from file
							{
								o=0;
								set_str[o]=fgetc(file);
								while(set_str[o]!='\t' && set_str[o]!='\n' && set_str[o]!=EOF)
								{
									o++;
									set_str[o]=fgetc(file);
								}
								o++;
								set_str[o]='\0';
								m++;
								if(m==1)
								{
									permX[i][j][k]=atof(set_str);
								}
								else if(m==2)
								{
									permY[i][j][k]=atof(set_str);
								}
								else
								{
									permZ[i][j][k]=atof(set_str);
								}
								++count;
								if(set_str[o-1]==EOF)	//if file is smaller than that we want ok set to false
								{
									if(count<nx*ny*nz/2)
									{
										MessageBox(L"The file for perm values is not right", L"Error", MB_OK|MB_ICONERROR);
										ok=false;	//If count is less than nx*ny*nz set to false and break for statments
										break;	//break while statement
									}
								}
							}
							if(ok == false)
							{
								break;	//break inner for statement (i)
							}
						}
						if(ok==false)
						{
							break;	//break middel for statement (j)
						}
					}
					if(ok==false)
					{
						break;	//break outes for statement (k)
					}
				}
				fclose(file);
			}
			else
			{
				MessageBox(L"Can not open Perm file",L"Error",MB_OK|MB_ICONERROR);
				ok=false;
			}
		}
		if(ok)
		{
			reservoirMaked = 1;
			reservoirMakedfirst = 1;
			CDialog::OnOK();
		}
	}
	else
	{
		MessageBox(L"Pleas Fill the required fields",L"Error",MB_OK|MB_ICONERROR);
		reservoirMaked = 0;
		return;
	}
}

void Reservoir::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//reservoirMaked = 0;
	OnCancel();
}
