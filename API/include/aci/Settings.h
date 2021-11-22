#pragma once

// aci header
#include <aci/aciAPIsharedTypes.h>
#include <aci/InterpreterObject.h>

// qt header
#include <qstring.h>

namespace aci {
	class ACI_API_EXPORT Settings : public InterpreterObject {
	public:
		static Settings *  instance(void);
		static void clearInstance(void);

		// ######################################################################################################

		// Base class functions

		virtual QString key(void) const override { return "settings"; }

		virtual bool handle(const QString& _command, const QStringList& _params) override;

		// ######################################################################################################

		// Getter

		const QString& dataPath(void) const { return m_dataPath; }

		// ######################################################################################################

		// Command handling

		void cmdConfiguration(void);

	protected:
		virtual void showCommandInfo(void) override;

	private:

		QString			m_dataPath;

		Settings();
		~Settings();
		Settings(Settings&) = delete;
		Settings& operator = (Settings&) = delete;
	};
}
