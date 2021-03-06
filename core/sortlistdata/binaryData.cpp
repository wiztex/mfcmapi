﻿#include <core/stdafx.h>
#include <core/sortlistdata/binaryData.h>
#include <core/sortlistdata/sortListData.h>

namespace sortlistdata
{
	void binaryData::init(sortListData* data, _In_opt_ LPSBinary lpOldBin)
	{
		if (!data) return;
		data->init(std::make_shared<binaryData>(lpOldBin));
	}

	binaryData::binaryData(_In_opt_ LPSBinary lpOldBin) noexcept
	{
		if (lpOldBin)
		{
			m_OldBin.cb = lpOldBin->cb;
			m_OldBin.lpb = lpOldBin->lpb;
		}
	}
} // namespace sortlistdata