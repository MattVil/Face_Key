--affiche liste
SELECT id_user, name, first_name, pseudo, mail
FROM Users
;

--recherche
SELECT id_user, name, first_name, pseudo, mail
FROM Users
WHERE name = 'vilain'          --WHERE '$choise' = '$search'
;

--infos generales user
SELECT *
FROM Users
WHERE id_user = 2   --changer exemple par variable
;

--liste accound d'un user
SELECT id_account, domain, login
FROM Account
  INNER JOIN Sites
    ON account.id_site = sites.id_site
WHERE account.id_user = 3     --changer exemple par variable
;

--infos generales d'un accound selectionné
SELECT account.id_account, account.id_site, domain, login, password, name_tag
FROM Account
  INNER JOIN Sites
    ON account.id_site = sites.id_site
  INNER JOIN Tags
    ON account.id_tag = tags.id_tag
WHERE account.id_user = 2 AND account.id_account = 4      --changer exemple par variable
;

--infos data d'un account
SELECT average_conn_time, last_loc, average_freq_conn, last_conn,  link_to_freq, link_to_time
FROM DataAccount
WHERE id_account = 7      --changer exemple par variable
;

--liste sharedAccount d'un user
SELECT id_sharedAccount, domain, name, first_name
FROM SharedAccount
  INNER JOIN Account
    ON sharedAccount.id_account = account.id_account
  INNER JOIN Sites
    ON account.id_site = sites.id_site
  INNER JOIN Users
    ON account.id_user = users.id_user
WHERE sharedAccount.id_receiver = 2     --changer exemple par variable
;

--infos generales d'un sharedAccount selectionné
SELECT id_sharedAccount, id_receiver, sharedAccount.id_account, expiration_date, domain, users.name, users.first_name, users.id_user
FROM SharedAccount
  INNER JOIN account
    ON sharedAccount.id_account = account.id_account
  INNER JOIN Sites
    ON account.id_site = sites.id_site
  INNER JOIN Tags
    ON account.id_tag = tags.id_tag
  INNER JOIN Users
    ON account.id_user = Users.id_user
WHERE sharedAccount.id_receiver = 2 AND sharedAccount.id_sharedaccount = 4   --changer exemple par variable
;

--infos data d'un sharedAccount
SELECT average_conn_time, last_loc, average_freq_conn, last_conn,  link_to_freq, link_to_time
FROM DataAccount
WHERE id_account = 3      --changer exemple par variable
;


--afficher la liste des sites
SELECT id_site, domain, nb_user_today
FROM Sites
;

--tier par ordre des plus fréquentés
SELECT id_site, domain, nb_user_today
FROM Sites
ORDER BY nb_user_today
;

--tier par ordre des moins frequentés
SELECT id_site, domain, nb_user_today
FROM Sites
ORDER BY nb_user_today
;

--rechercher selon un domaine
SELECT id_site, domain, nb_user_today
FROM Sites
WHERE domain = 'trello.com'     --changer exemple par variable
;

--infos sur un site
SELECT *
FROM SSites
WHERE id_site = '3'       --changer exemple par variable
;

--nb utilisateur par site
SELECT COUNT(id_account)
FROM Account
  INNER JOIN Sites
    ON Account.id_site = Sites.id_site
WHERE domain = 'google.com'       --changer exemple par variable
;

--somme du temps d'utilisation + nb utilisateurs d'un site pour calcul moyenne
SELECT SUM(average_conn_time) AS summe, COUNT(average_conn_time) AS count
FROM Account
  INNER JOIN Sites
    ON Account.id_site = Sites.id_site
  INNER JOIN DataAccount
    ON Account.id_account = DataAccount.id_account
WHERE domain = 'facebook.com'
;

--liste des positions GPS des utilisateurs d'un site
SELECT last_loc
FROM DataAccount
  INNER JOIN Account
    ON DataAccount.id_account = Account.id_account
  INNER JOIN Sites
    ON Account.id_site = Sites.id_site
WHERE domain = 'twitter.com'
;

--valeur max de la clé id_account dans le tableau account
SELECT MAX(id_account) AS maximum
FROM Account
;

--liste account + PaymentAccount
SELECT account.id_account, account.login, domain, account.password, account.id_user, account.id_tag, bank, rib, card_num, cryptogram
FROM Account
  LEFT JOIN PaymentAccount
    ON account.id_account = paymentaccount.id_account
  INNER JOIN Sites
    ON account.id_site = sites.id_site
WHERE account.id_user = 3
;

--liste des tags d'un site
SELECT DISTINCT name_tag
FROM Account
  INNER JOIN Tags
    ON account.id_tag = tags.id_tag
  INNER JOIN sites
    ON account.id_site = Sites.id_site
WHERE account.id_site = 3
;

--liste des personnes avec qui est partagé un compte
SELECT Users.id_user, Users.name, Users.first_name, Users.pseudo
FROM SharedAccount
  INNER JOIN Users
    ON Users.id_user = SharedAccount.id_receiver
WHERE SharedAccount.id_account = 9
;

--dernier compte utilisé par un user
SELECT MAX(last_conn)
FROM(
  SELECT Account.id_account, domain, last_conn
  FROM Account
    INNER JOIN sites
      ON account.id_site = sites.id_site
    INNER JOIN DataAccount
      ON account.id_account = DataAccount.id_account
  WHERE account.id_user = 2
)AS account_user;



--(idee panel)
--ajouter des stats sur l'apply FaceKey sur page principale genre graph de l'evolution du nombre d'utilisateurs




COMMIT;
