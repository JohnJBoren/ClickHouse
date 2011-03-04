#include <Poco/Util/Application.h>

#include <mysqlxx/Connection.h>
#include <mysqlxx/Exception.h>


namespace mysqlxx
{

Connection::Connection()
{
	is_connected = false;
}

Connection::Connection(
	const char* db,
	const char* server,
	const char* user,
	const char* password,
	unsigned int port)
{
	is_connected = false;
	connect(db, server, user, password, port);
}

Connection::Connection(const std::string & config_name)
{
	is_connected = false;
	Poco::Util::LayeredConfiguration & cfg = Poco::Util::Application::instance().config();

	std::string db 			= cfg.getString(config_name + ".db");
	std::string server 		= cfg.getString(config_name + ".host");
	std::string user 		= cfg.getString(config_name + ".user");
	std::string password	= cfg.getString(config_name + ".password");
	unsigned port			= cfg.getInt(config_name + ".port");
	
	connect(db.c_str(), server.c_str(), user.c_str(), password.c_str(), port);
}

Connection::~Connection()
{
	disconnect();
}

void Connection::connect(const char* db,
	const char* server,
	const char* user,
	const char* password,
	unsigned int port)
{
	if (is_connected)
		disconnect();

	if (!mysql_init(&driver))
		throw ConnectionFailed(mysql_error(&driver), mysql_errno(&driver));

	if (!mysql_real_connect(&driver, server, user, password, db, port, NULL, driver.client_flag))
		throw ConnectionFailed(mysql_error(&driver), mysql_errno(&driver));

	is_connected = true;
}

bool Connection::connected() const
{
	return is_connected;
}

void Connection::disconnect()
{
	if (!is_connected)
		return;

	mysql_close(&driver);
	memset(&driver, 0, sizeof(driver));
	is_connected = false;
}

bool Connection::ping()
{
	return is_connected && !mysql_ping(&driver);
}

Query Connection::query(const std::string & str)
{
	return Query(*this, str);
}

MYSQL & Connection::getDriver()
{
	return driver;
}

}

