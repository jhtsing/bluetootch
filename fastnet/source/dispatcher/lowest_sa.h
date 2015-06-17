#ifndef __NAMED_PIPE_LOWEST_SA_H__
#define __NAMED_PIPE_LOWEST_SA_H__
#include <Sddl.h>
#include <Windows.h>
 namespace named_pipe
 {
	class CLowSA
	{
	public:
		CLowSA():m_pTmpSD(NULL), m_bError(FALSE)
		{
			m_bError = CreateALowSA(&m_SA, &m_SD)?FALSE:TRUE;
		}

		~CLowSA()
		{
			if (m_pTmpSD != NULL)
			{
				::LocalFree(m_pTmpSD);
				m_pTmpSD = NULL;
			}
		}
		LPSECURITY_ATTRIBUTES GetSA()
		{
			if (m_bError) 
				return NULL;
			return &m_SA;
		}
		operator LPSECURITY_ATTRIBUTES ()
		{
			return GetSA();
		}

	protected:
		// ------------------------------------------------------------------------
		// 函数		: CreateALowSA
		// 功能		: 创建一个低权限的安全属性体
		// 返回值	: BOOL 
		// 参数		: SECURITY_ATTRIBUTES *pSA
		// 参数		: SECURITY_DESCRIPTOR *pSecurityD
		// 附注		: 
		// -------------------------------------------------------------------------
		BOOL CLowSA::CreateALowSA(SECURITY_ATTRIBUTES *pSA, SECURITY_DESCRIPTOR *pSecurityD)
		{
#define LOW_INTEGRITY_SDDL_SACL TEXT("S:(ML;;NW;;;LW)")

			if (m_pTmpSD != NULL)
			{
				::LocalFree(m_pTmpSD);
				m_pTmpSD = NULL;
			}

			// 初始化安全属性
			BOOL bFuncRet = FALSE;
			PACL pSacl = NULL;                  
			BOOL fSaclPresent = FALSE;
			BOOL fSaclDefaulted = FALSE;

			// 如果是vista则创建一个低权限的共享内存
			pSA->nLength = sizeof(SECURITY_ATTRIBUTES);
			pSA->bInheritHandle = FALSE;
			pSA->lpSecurityDescriptor = pSecurityD;
			do 
			{
				// 初始化一个安全描述符，第二个参数必须为SECURITY_DESCRIPTOR_REVISION
				bFuncRet = ::InitializeSecurityDescriptor(pSA->lpSecurityDescriptor, SECURITY_DESCRIPTOR_REVISION);
				if (!bFuncRet)
				{
					break;
				}

				// 设置DACL（自由访问控制表），此处设置了一个空的dacl，并且不使用默认的dacl
				bFuncRet = ::SetSecurityDescriptorDacl(pSA->lpSecurityDescriptor, TRUE, 0, FALSE);
				if (!bFuncRet)
				{
					break;
				}
				// 将一个安全描述符字符串转换成安全描述符，安全描述符字符串是个格式化的字符串
				// 第二个参数恒定为SDDL_REVISION_1
				// 第四个参数是返回安全描述符字节数，可以为空
				// 这个API真长啊
				bFuncRet = ::ConvertStringSecurityDescriptorToSecurityDescriptor(
					LOW_INTEGRITY_SDDL_SACL, 
					SDDL_REVISION_1, 
					&m_pTmpSD, NULL);	// xp下这个api会失败。
				if (!bFuncRet)
				{
					bFuncRet = TRUE;
					break;
				}

				// 设置SACL（系统访问控制列表）
				bFuncRet = ::GetSecurityDescriptorSacl(m_pTmpSD, &fSaclPresent, &pSacl, &fSaclDefaulted);
				if (!bFuncRet)
				{
					break;
				}
				bFuncRet = ::SetSecurityDescriptorSacl(pSA->lpSecurityDescriptor, TRUE, pSacl, FALSE);
				if (!bFuncRet)
				{
					break;
				}

				bFuncRet = TRUE;

			} while(0);

			return bFuncRet;
		}
	private:
		BOOL m_bError;
		SECURITY_ATTRIBUTES m_SA;
		SECURITY_DESCRIPTOR m_SD;
		PSECURITY_DESCRIPTOR m_pTmpSD;
	};
}

#endif//BD_IPC_H__
