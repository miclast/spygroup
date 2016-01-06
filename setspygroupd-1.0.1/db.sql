create database ast default charset utf8;
grant all privileges on ast.* to ast_user@localhost identified by 'ast_pass';
flush privileges;
connect ast
CREATE TABLE `spygroups` ( `id` int(11) unsigned NOT NULL, `extension` varchar(80) NOT NULL DEFAULT '', UNIQUE KEY `ksg` (`id`, `extension`));
CREATE TABLE `group_pass` ( `id` int(11) unsigned NOT NULL, `pass` varchar(80) NOT NULL DEFAULT '', UNIQUE KEY `kip` (`id`, `pass`));
