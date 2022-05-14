#pragma once

// aci header
#include <aci/aciSharedDataTypes.h>
#include <aci/InterpreterObject.h>

// qt header
#include <string>

class Settings : public aci::InterpreterObject {
public:
	static Settings *  instance(void);
	static void clearInstance(void);

	// ######################################################################################################

	// Base class functions

	virtual std::wstring key(void) const override { return L"settings"; }

	virtual bool handle(const std::wstring& _command, const std::vector<std::wstring>& _params) override;

	// ######################################################################################################

	// Getter

	const std::wstring& dataPath(void) const { return m_dataPath; }

	// ######################################################################################################

	// Command handling

	void cmdConfiguration(void);

protected:
	virtual void showCommandInfo(void) override;

private:

	std::wstring			m_dataPath;

	Settings();
	~Settings();
	Settings(Settings&) = delete;
	Settings& operator = (Settings&) = delete;
};
