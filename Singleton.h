#pragma once

// �̰� Ŭ�������ٰ� ������ �̱������� ���� �� �ִ�
// ���� : Ŭ������ �ְ� �ٸ� �����ڴ� ��� ���־ߵ�

// ������ ������
#define DECLARE_SINGLETON(className)\
private:\
	className() {}\
public:\
	static className* getInstance()\
	{\
		static className s_instance;\
		return &s_instance;\
	}\

// �����Ҵ� - �� ������
#define CREATE_SINGLETON(className)\
private:\
	className();\
	~className();\
	className(const className& rhs) = delete;\
	className operator = (className& rObj) = delete;\
	static className* m_pInstance;\
public:\
	static className* getInstance()\
	{\
		if (!m_pInstance)\
		{\
			m_pInstance = new className;\
		}\
		return m_pInstance;\
	}\
	static void	destroyInstance()\
	{\
		if (m_pInstance)\
		{\
			delete m_pInstance;\
			m_pInstance = nullptr;\
		}\
	}\

// �ν��Ͻ� �ҷ����� ��
#define GET_SINGLETON(className) className::getInstance()
// �����Ҵ� ����
#define DELETE_SINGLETON(className) className::destroyInstance()