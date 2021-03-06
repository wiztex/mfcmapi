#include <core/stdafx.h>
#include <core/smartview/NickNameCache.h>

namespace smartview
{
	SRowStruct::SRowStruct(const std::shared_ptr<binaryParser>& parser)
	{
		cValues = blockT<DWORD>::parse(parser);

		if (*cValues)
		{
			if (*cValues < _MaxEntriesSmall)
			{
				lpProps.EnableNickNameParsing();
				lpProps.SetMaxEntries(*cValues);
				lpProps.smartViewParser::parse(parser, false);
			}
		}
	} // namespace smartview

	void NickNameCache::parse()
	{
		m_Metadata1 = blockBytes::parse(m_Parser, 4);
		m_ulMajorVersion = blockT<DWORD>::parse(m_Parser);
		m_ulMinorVersion = blockT<DWORD>::parse(m_Parser);
		m_cRowCount = blockT<DWORD>::parse(m_Parser);

		if (*m_cRowCount)
		{
			if (*m_cRowCount < _MaxEntriesEnormous)
			{
				m_lpRows.reserve(*m_cRowCount);
				for (DWORD i = 0; i < *m_cRowCount; i++)
				{
					m_lpRows.emplace_back(std::make_shared<SRowStruct>(m_Parser));
				}
			}
		}

		m_cbEI = blockT<DWORD>::parse(m_Parser);
		m_lpbEI = blockBytes::parse(m_Parser, *m_cbEI, _MaxBytes);
		m_Metadata2 = blockBytes::parse(m_Parser, 8);
	}

	void NickNameCache::parseBlocks()
	{
		setRoot(L"Nickname Cache\r\n");
		addLabeledChild(L"Metadata1 = ", m_Metadata1);

		addChild(m_ulMajorVersion, L"Major Version = %1!d!\r\n", m_ulMajorVersion->getData());
		addChild(m_ulMinorVersion, L"Minor Version = %1!d!\r\n", m_ulMinorVersion->getData());
		addChild(m_cRowCount, L"Row Count = %1!d!", m_cRowCount->getData());

		if (!m_lpRows.empty())
		{
			auto i = DWORD{};
			for (const auto& row : m_lpRows)
			{
				terminateBlock();
				if (i > 0) addBlankLine();
				auto rowBlock = std::make_shared<block>(strings::formatmessage(L"Row %1!d!\r\n", i));
				addChild(rowBlock);
				rowBlock->addChild(row->cValues, L"cValues = 0x%1!08X! = %1!d!\r\n", row->cValues->getData());
				rowBlock->addChild(row->lpProps.getBlock());

				i++;
			}
		}

		terminateBlock();
		addBlankLine();

		addLabeledChild(L"Extra Info = ", m_lpbEI);

		addLabeledChild(L"Metadata 2 = ", m_Metadata2);
	}
} // namespace smartview