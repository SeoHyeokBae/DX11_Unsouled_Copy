#pragma once

#define SINGLE(ClassType)	private:\
								ClassType();\
								~ClassType();\
							friend class CSingleton<ClassType>; // 생성자를 private로 막아서 싱글톤에서 생성시 접근을 위해

#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()
