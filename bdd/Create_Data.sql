DELETE FROM PaymentAccount;
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
INSERT INTO Sites (domain, login_input, password_input, nb_user_today, nb_user_week, nb_user_month) VALUES ('facebook.com', 'login', 'password', '4', '5', '5');
INSERT INTO Sites (domain, login_input, password_input, nb_user_today, nb_user_week, nb_user_month) VALUES ('twitter.com', 'login', 'password', '4', '4', '4');
INSERT INTO Sites (domain, login_input, password_input, nb_user_today, nb_user_week, nb_user_month) VALUES ('google.com', 'login', 'password', '5', '5', '5');
INSERT INTO Sites (domain, login_input, password_input, nb_user_today, nb_user_week, nb_user_month) VALUES ('trello.com', 'login', 'password', '1', '2', '3');
INSERT INTO Sites (domain, login_input, password_input, nb_user_today, nb_user_week, nb_user_month) VALUES ('evernote.com', 'login', 'password', '0', '0', '2');
INSERT INTO Sites (domain, login_input, password_input, nb_user_today, nb_user_week, nb_user_month) VALUES ('nytimes.com', 'login', 'password', '1', '1', '1');
INSERT INTO Sites (domain, login_input, password_input, nb_user_today, nb_user_week, nb_user_month) VALUES ('amazon.com', 'login', 'password', '0', '2', '4');
INSERT INTO Sites (domain, login_input, password_input, nb_user_today, nb_user_week, nb_user_month) VALUES ('spotify.com', 'login', 'password', '2', '2', '2');
INSERT INTO Sites (domain, login_input, password_input, nb_user_today, nb_user_week, nb_user_month) VALUES ('apple.com', 'login', 'password', '0', '0', '1');

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
INSERT INTO Account (id_site, login, password, id_user, id_tag)
	VALUES (5, 'matthieu.vilain@etu.u-cergy.fr', 'psswrdEVRNTMV', 2, 4);
INSERT INTO Account (id_site, login, password, id_user, id_tag)
	VALUES (6, 'matthieu.vilain@etu.u-cergy.fr', 'psswrdNYTMV', 2, 6);
INSERT INTO Account (id_site, login, password, id_user, id_tag)
	VALUES (3, 'matthieu.vilain@etu.u-cergy.fr', 'psswrdGGLMV', 2, 1);
INSERT INTO Account (id_site, login, password, id_user, id_tag)
	VALUES (9, 'louis.lharidon@etu.u-cergy.fr', 'psswrdAPPLLL', 3, 7);
INSERT INTO Account (id_site, login, password, id_user, id_tag)
	VALUES (6, 'louis.lharidon@etu.u-cergy.fr', 'psswrdNYTLL', 3, 6);
INSERT INTO Account (id_site, login, password, id_user, id_tag)
	VALUES (3, 'louis.lharidon@etu.u-cergy.fr', 'psswrdGGLLL', 3, 2);
INSERT INTO Account (id_site, login, password, id_user, id_tag)
	VALUES (2, 'jerome.vabois@etu.u-cergy.fr', 'psswrdTWTJV', 4, 3);
INSERT INTO Account (id_site, login, password, id_user, id_tag)
	VALUES (8, 'jerome.vabois@etu.u-cergy.fr', 'psswrdSPTFJV', 4, 8);
INSERT INTO Account (id_site, login, password, id_user, id_tag)
	VALUES (1, 'jerome.vabois@etu.u-cergy.fr', 'psswrdFBJV', 4, 3);
INSERT INTO Account (id_site, login, password, id_user, id_tag)
	VALUES (1, 'guillaume.riguidel@etu.u-cergy.fr', 'psswrdFBGR', 5, 3);
INSERT INTO Account (id_site, login, password, id_user, id_tag)
	VALUES (7, 'guillaume.riguidel@etu.u-cergy.fr', 'psswrdAMZGR', 5, 7);
INSERT INTO Account (id_site, login, password, id_user, id_tag)
	VALUES (2, 'guillaume.riguidel@etu.u-cergy.fr', 'psswrdTWTGR', 5, 2);

/*SharedAccount Insertion*/
INSERT INTO SharedAccount (expiration_date, id_account, id_receiver)
	VALUES ('2018/12/31', 9, 1);
INSERT INTO SharedAccount (expiration_date, id_account, id_receiver)
	VALUES ('2018/12/31', 11, 2);
INSERT INTO SharedAccount (expiration_date, id_account, id_receiver)
	VALUES ('2018/12/31', 4, 1);
INSERT INTO SharedAccount (expiration_date, id_account, id_receiver)
	VALUES ('2018/12/31', 9, 2);
INSERT INTO SharedAccount (expiration_date, id_account, id_receiver)
	VALUES ('2018/12/31', 9, 4);
INSERT INTO SharedAccount (expiration_date, id_account, id_receiver)
	VALUES ('2018/12/31', 9, 5);
INSERT INTO SharedAccount (expiration_date, id_account, id_receiver)
	VALUES ('2018/12/31', 14, 3);

/*DataAccount Insertion*/
INSERT INTO DataAccount (id_account, average_conn_time, last_loc, average_freq_conn, last_conn, link_to_freq, link_to_time)
	VALUES (1, '00:31:22', '46°37.5110N, 2°14.8760E', 10, '2017-11-6 16:34:20', NULL, NULL);
INSERT INTO DataAccount (id_account, average_conn_time, last_loc, average_freq_conn, last_conn, link_to_freq, link_to_time)
	VALUES (2, '00:30:52', '49°37.5120N, 2°34.8760E', 10, '2017-12-7 10:34:20', NULL, NULL);
INSERT INTO DataAccount (id_account, average_conn_time, last_loc, average_freq_conn, last_conn, link_to_freq, link_to_time)
	VALUES (3, '00:11:25', '41°37.5120N, 2°24.0760E', 10, '2017-12-1 17:36:20', NULL, NULL);
INSERT INTO DataAccount (id_account, average_conn_time, last_loc, average_freq_conn, last_conn, link_to_freq, link_to_time)
	VALUES (4, '00:52:36', '46°37.5120N, 2°24.9760E', 10, '2017-9-24 9:54:20', NULL, NULL);
INSERT INTO DataAccount (id_account, average_conn_time, last_loc, average_freq_conn, last_conn, link_to_freq, link_to_time)
	VALUES (5, '00:42:42', '36°37.5120N, 2°14.8760E', 10, '2017-12-14 12:14:20', NULL, NULL);
INSERT INTO DataAccount (id_account, average_conn_time, last_loc, average_freq_conn, last_conn, link_to_freq, link_to_time)
	VALUES (6, '00:01:25', '46°37.1120N, 2°24.8760E', 10, '2017-10-16 07:36:20', NULL, NULL);
INSERT INTO DataAccount (id_account, average_conn_time, last_loc, average_freq_conn, last_conn, link_to_freq, link_to_time)
	VALUES (7, '00:05:30', '46°37.5120N, 2°23.8760E', 10, '2017-2-17 10:05:20', NULL, NULL);
INSERT INTO DataAccount (id_account, average_conn_time, last_loc, average_freq_conn, last_conn, link_to_freq, link_to_time)
	VALUES (8, '01:41:52', '46°37.5120N, 2°21.8760E', 10, '2017-2-9 19:54:20', NULL, NULL);
INSERT INTO DataAccount (id_account, average_conn_time, last_loc, average_freq_conn, last_conn, link_to_freq, link_to_time)
	VALUES (9, '00:51:59', '26°37.5120N, 2°24.8760E', 10, '2017-1-26 23:34:20', NULL, NULL);
INSERT INTO DataAccount (id_account, average_conn_time, last_loc, average_freq_conn, last_conn, link_to_freq, link_to_time)
	VALUES (10, '00:11:15', '46°37.5120N, 2°22.8760E', 10, '2017-1-30 22:37:20', NULL, NULL);
INSERT INTO DataAccount (id_account, average_conn_time, last_loc, average_freq_conn, last_conn, link_to_freq, link_to_time)
	VALUES (11, '00:45:42', '46°37.5120N, 2°24.9760E', 10, '2017-12-14 21:54:20', NULL, NULL);
INSERT INTO DataAccount (id_account, average_conn_time, last_loc, average_freq_conn, last_conn, link_to_freq, link_to_time)
	VALUES (12, '00:20:15', '45°37.5120N, 2°24.8760E', 10, '2017-7-19 03:34:20', NULL, NULL);
INSERT INTO DataAccount (id_account, average_conn_time, last_loc, average_freq_conn, last_conn, link_to_freq, link_to_time)
	VALUES (13, '00:51:24', '56°37.5120N, 2°24.5760E', 10, '2017-3-11 09:24:20', NULL, NULL);
INSERT INTO DataAccount (id_account, average_conn_time, last_loc, average_freq_conn, last_conn, link_to_freq, link_to_time)
	VALUES (14, '00:06:52', '46°37.5120N, 1°24.1760E', 10, '2017-4-13 09:12:20', NULL, NULL);
INSERT INTO DataAccount (id_account, average_conn_time, last_loc, average_freq_conn, last_conn, link_to_freq, link_to_time)
	VALUES (15, '00:36:44', '43°37.3120N, 2°24.2760E', 10, '2017-5-4 16:14:20', NULL, NULL);

/*PaymentAccount Insertion*/
INSERT INTO PaymentAccount VALUES (120 , 9, 'louis.lharidon@etu.u-cergy.fr', 'psswrdAPPLLL', 3, 7, 'Crédit Agricole', 'FR50RABO0309252518', '5592610767473472', '345');
INSERT INTO PaymentAccount VALUES (121 , 7, 'guillaume.riguidel@etu.u-cergy.fr', 'psswrdAMZGR', 5, 7, 'BNP Paribas', 'FR87SNSB0960200234', '4539386035030936', '290');
