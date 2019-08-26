#pragma once
#include "ScreenPoint.h"

/*
DisplaySetting�� WindowsApp.cpp���� static ������ �ȵΰ� �̱������� ����� ����?
 > �ʱ�ȭ ���� ����. ��� �ʱ�ȭ �Ǵ°�
  static���� �ϸ� ���α׷� ����� ������ �ʱ�ȭ�� ��  > ȣ������ ������ �𸣴µ� �ε��� �ð��� ���� �ɸ��� �� ��
  �̱������� �ϸ� ȣ��Ǵ� �������� �ʱ�ȭ�� �� 
  ���ϴ� �������� ������ ȣ���ϴ� ��� ?lazy intialization (������ �ʱ�ȭ ���) ������ �˾Ƶθ� ����.
*/

class DisplaySetting 
{
	public:
	DisplaySetting(DisplaySetting const&) = delete;
	void operator=(DisplaySetting const&) = delete;
	static DisplaySetting& Inst()
	{
		static DisplaySetting instance;
		return instance;
	}

public:
	void SetSize(ScreenPoint inSize) { size = inSize; }
	ScreenPoint GetSize() { return size; }

private:
	ScreenPoint size;

private:
	DisplaySetting() { }
	~DisplaySetting() { }
};