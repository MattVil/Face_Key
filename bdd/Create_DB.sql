DELETE FROM User;
DELETE FROM Account;
DELETE FROM SharedAccount;
DELETE FROM Settings;

DROP TABLE User;
DROP TABLE Account;
DROP TABLE SharedAccount;
DROP TABLE Settings;

CREATE TABLE User{
	id_user SERIAL;
	pseudo VARCHAR2(20);
	mail_adr VARCHAR2(75);
	password VARCHAR2(100);
	CONSTRAINT user_pk PRIMARY KEY (id_user);
}

CREATE TABLE Account{
	id_account SERIAL;
	site VARCHAR2(100);
	login VARCHAR2(75);
	password VARCHAR2(100);
	id_user SERIAL REFERENCES User(id_user);
	CONSTRAINT account_pk PRIMARY KEY (id_account);
}

CREATE TABLE SharedAccount{
	id_share SERIAL;
	expire_date DATE;
	id_account SERIAL REFERENCES Account(id_account);
	id_user SERIAL REFERENCES User(id_user);
	CONSTRAINT sharedaccount_pk PRIMARY KEY (id_share);
}

CREATE TABLE Settings{
	id_user SERIAL REFERENCES User(id_user);
	language VARCHAR2(20);
	CONSTRAINT settings_pk PRIMARY KEY (id_user)
}