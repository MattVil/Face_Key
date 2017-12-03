DELETE FROM DataAccount;
DELETE FROM SharedAccount;
DELETE FROM Account;
DELETE FROM Tags;
DELETE FROM Sites;
DELETE FROM Users;

/*Tags Insertion*/
INSERT INTO Tags (name_tag) VALUES ('Personnal');
INSERT INTO Tags (name_tag) VALUES ('Work');
INSERT INTO Tags (name_tag) VALUES ('Social Network');
INSERT INTO Tags (name_tag) VALUES ('Tool');
INSERT INTO Tags (name_tag) VALUES ('Mail');
INSERT INTO Tags (name_tag) VALUES ('News');
INSERT INTO Tags (name_tag) VALUES ('Shop');
INSERT INTO Tags (name_tag) VALUES ('Music');

/*Sites Insertion*/
INSERT INTO Sites (domain) VALUES ('facebook.com');
INSERT INTO Sites (domain) VALUES ('twitter.com');
INSERT INTO Sites (domain) VALUES ('google.com');
INSERT INTO Sites (domain) VALUES ('trello.com');
INSERT INTO Sites (domain) VALUES ('evernote.com');
INSERT INTO Sites (domain) VALUES ('nytimes.com');
INSERT INTO Sites (domain) VALUES ('amazon.com');
INSERT INTO Sites (domain) VALUES ('spotify.com');

/*Users Insertion*/
INSERT INTO Users (name, first_name, gender, pseudo, mail, password, language, creation_date)
	VALUES ('Quentin', 'GERARD', TRUE, 'QuentGrd', 'quentin.gerard@etu.u-cergy.fr', 'psno97s&!s§', 'French', '1970/01/01');
INSERT INTO Users (name, first_name, gender, pseudo, mail, password, language, creation_date)
	VALUES ('Matthieu', 'VILAIN', TRUE, 'MattVil', 'matthieu.vilain@etu.u-cergy.fr', '7hU?2@lO', 'French', '1970/01/01');
INSERT INTO Users (name, first_name, gender, pseudo, mail, password, language, creation_date)
	VALUES ('Louis', 'LHARIDON', FALSE, 'feudecamp', 'louis.lharidon@etu.u-cergy.fr', 'azerty', 'French', '1970/01/01');
INSERT INTO Users (name, first_name, gender, pseudo, mail, password, language, creation_date)
	VALUES ('Jérôme', 'VABOIS', TRUE, 'JeromeVab', 'jerome.vabois@etu.u-cergy.fr', 'password', 'French', '1970/01/01');
INSERT INTO Users (name, first_name, gender, pseudo, mail, password, language, creation_date)
	VALUES ('Guillaume', 'RIGUIDEL', TRUE, 'DJGuiGui', 'guillaume.riguidel@etu.u-cergy.fr', '123456', 'French', '1970/01/01');

/*Account Insertion*/
INSERT INTO Account (id_site, login, password, id_user, id_tag) 
	VALUES (1, 'quentin.gerard@etu.u-cergy.fr', 'psswrdFBQG', 1, 3);
INSERT INTO Account (id_site, login, password, id_user, id_tag) 
	VALUES (8, 'quentin.gerard@etu.u-cergy.fr', 'psswrdSPTFQG', 1, 8);
INSERT INTO Account (id_site, login, password, id_user, id_tag) 
	VALUES (4, 'quentin.gerard@etu.u-cergy.fr', 'psswrdTRLLQG', 1, 4);

