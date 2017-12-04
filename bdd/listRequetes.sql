--affiche liste
SELECT id_user, name, first_name, pseudo, mail
FROM Users
;

--recherche
SELECT id_user, name, first_name, pseudo, mail
FROM Users
WHERE name = 'vilain'          --WHERE '$choise' = '$search'

--infos generales user
SELECT *
FROM Users
WHERE id_user = 2

--liste accound d'un user
SELECT id_account, domain, login
FROM Account
  INNER JOIN Sites
    ON account.id_site = sites.id_site
WHERE account.id_user = 3
;

--infos generales d'un accound selectionné
SELECT account.id_account, account.id_site, domain, login, password, name_tag
FROM Account
  INNER JOIN Sites
    ON account.id_site = sites.id_site
  INNER JOIN Tags
    ON account.id_tag = tags.id_tag
WHERE account.id_user = 2 AND account.id_account = 4
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
WHERE sharedAccount.id_receiver = 3
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
WHERE sharedAccount.id_receiver = 2 AND sharedAccount.id_account = 4
;


COMMIT;
