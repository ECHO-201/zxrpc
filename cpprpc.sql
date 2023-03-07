/*
Navicat MySQL Data Transfer

Source Server         : bdzny
Source Server Version : 50739
Source Host           : 120.48.82.123:3306
Source Database       : cpprpc

Target Server Type    : MYSQL
Target Server Version : 50739
File Encoding         : 65001

Date: 2023-03-05 19:16:33
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for ERROR_log
-- ----------------------------
DROP TABLE IF EXISTS `ERROR_log`;
CREATE TABLE `ERROR_log` (
  `id` int(16) unsigned NOT NULL AUTO_INCREMENT,
  `level` varchar(10) COLLATE utf8_bin DEFAULT NULL,
  `record` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  `ip` varchar(10) COLLATE utf8_bin DEFAULT NULL,
  `port` varchar(8) COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- ----------------------------
-- Table structure for FATAL_log
-- ----------------------------
DROP TABLE IF EXISTS `FATAL_log`;
CREATE TABLE `FATAL_log` (
  `id` int(16) unsigned NOT NULL AUTO_INCREMENT,
  `level` varchar(10) COLLATE utf8_bin DEFAULT NULL,
  `record` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  `ip` varchar(10) COLLATE utf8_bin DEFAULT NULL,
  `port` varchar(8) COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- ----------------------------
-- Table structure for INFO_log
-- ----------------------------
DROP TABLE IF EXISTS `INFO_log`;
CREATE TABLE `INFO_log` (
  `id` int(16) unsigned NOT NULL AUTO_INCREMENT,
  `level` varchar(10) COLLATE utf8_bin DEFAULT NULL,
  `record` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  `ip` varchar(10) COLLATE utf8_bin DEFAULT NULL,
  `port` varchar(8) COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=126 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- ----------------------------
-- Table structure for LOGIN
-- ----------------------------
DROP TABLE IF EXISTS `LOGIN`;
CREATE TABLE `LOGIN` (
  `id` int(12) unsigned NOT NULL AUTO_INCREMENT,
  `userid` varchar(16) COLLATE utf8_bin DEFAULT NULL,
  `macaddr` varchar(32) COLLATE utf8_bin DEFAULT NULL,
  `localaddr` varchar(32) COLLATE utf8_bin DEFAULT NULL,
  `time` varchar(32) COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- ----------------------------
-- Table structure for op_log
-- ----------------------------
DROP TABLE IF EXISTS `op_log`;
CREATE TABLE `op_log` (
  `id` varchar(12) COLLATE utf8_bin DEFAULT NULL,
  `time` varchar(12) COLLATE utf8_bin DEFAULT NULL,
  `op` varchar(8) COLLATE utf8_bin DEFAULT NULL,
  `record` varchar(32) COLLATE utf8_bin DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- ----------------------------
-- Table structure for op_service
-- ----------------------------
DROP TABLE IF EXISTS `op_service`;
CREATE TABLE `op_service` (
  `id` varchar(12) COLLATE utf8_bin DEFAULT NULL,
  `time` varchar(12) COLLATE utf8_bin DEFAULT NULL,
  `op` varchar(32) COLLATE utf8_bin DEFAULT NULL,
  `service` varchar(64) COLLATE utf8_bin DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- ----------------------------
-- Table structure for REGISTER_log
-- ----------------------------
DROP TABLE IF EXISTS `REGISTER_log`;
CREATE TABLE `REGISTER_log` (
  `id` int(12) unsigned NOT NULL AUTO_INCREMENT,
  `userid` varchar(16) COLLATE utf8_bin DEFAULT NULL,
  `macaddr` varchar(32) COLLATE utf8_bin DEFAULT NULL,
  `localaddr` varchar(32) COLLATE utf8_bin DEFAULT NULL,
  `time` varchar(32) COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- ----------------------------
-- Table structure for rpc_serv
-- ----------------------------
DROP TABLE IF EXISTS `rpc_serv`;
CREATE TABLE `rpc_serv` (
  `id` int(8) NOT NULL AUTO_INCREMENT,
  `name` varchar(16) COLLATE utf8_bin DEFAULT NULL,
  `cnt` int(32) DEFAULT NULL,
  `succ` int(32) DEFAULT NULL,
  `fail` int(32) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=24 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- ----------------------------
-- Table structure for user
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `id` varchar(10) COLLATE utf8_bin NOT NULL,
  `name` varchar(16) COLLATE utf8_bin DEFAULT NULL,
  `passwd` varchar(16) COLLATE utf8_bin DEFAULT NULL,
  `phone` varchar(16) COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- ----------------------------
-- Table structure for WARN_log
-- ----------------------------
DROP TABLE IF EXISTS `WARN_log`;
CREATE TABLE `WARN_log` (
  `id` int(16) unsigned NOT NULL AUTO_INCREMENT,
  `level` varchar(10) COLLATE utf8_bin DEFAULT NULL,
  `record` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  `ip` varchar(10) COLLATE utf8_bin DEFAULT NULL,
  `port` varchar(8) COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=47 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
