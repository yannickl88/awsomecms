-- phpMyAdmin SQL Dump
-- version 2.11.7
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generatie Tijd: 04 Mei 2009 om 22:01
-- Server versie: 5.0.51
-- PHP Versie: 5.2.6

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

--
-- Database: `SlyFox Component Framework`
--

-- --------------------------------------------------------

--
-- Tabel structuur voor tabel `forms`
--

CREATE TABLE IF NOT EXISTS `forms` (
  `form_id` int(11) NOT NULL auto_increment,
  `form_name` varchar(100) NOT NULL,
  `form_to` varchar(100) NOT NULL,
  `form_bcc` varchar(100) NOT NULL,
  `form_subject` varchar(100) NOT NULL,
  `form_thanks` varchar(100) NOT NULL,
  PRIMARY KEY  (`form_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=4 ;

-- --------------------------------------------------------

--
-- Tabel structuur voor tabel `form_fields`
--

CREATE TABLE IF NOT EXISTS `form_fields` (
  `field_id` int(11) NOT NULL auto_increment,
  `field_form` int(11) NOT NULL,
  `field_name` varchar(100) NOT NULL,
  `field_code` varchar(100) NOT NULL,
  `field_type` tinyint(4) NOT NULL,
  `field_required` tinyint(4) NOT NULL,
  PRIMARY KEY  (`field_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=60 ;
