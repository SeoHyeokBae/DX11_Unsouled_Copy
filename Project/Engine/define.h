#pragma once

#define SINGLE(ClassType)	private:\
								ClassType();\
								~ClassType();\
							friend class CSingleton<ClassType>; // �����ڸ� private�� ���Ƽ� �̱��濡�� ������ ������ ����

#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()
