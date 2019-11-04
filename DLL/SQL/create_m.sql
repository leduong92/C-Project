/*
  TNC Production process control system
  create.sql
  dateabase create script for PostgreSQL
  2006.05.12 
*/

/* master files */
/* #1 job classification master */
create table tm_job_class
(
	job_class_code char(3),
	job_class_name varchar(20),
	fp_sign	smallint,
	entry_date char(8),
	update_date char(8),
	primary key(job_class_code)
);
/* #2 function master */

create table tm_function
(
	function_no char(2),
	function_name varchar(30),
	entry_date char(8),
	update_date char(8),
	primary key(function_no)
);

/* #3 input element master */
create table tm_element
(
	element_code char(3),
	element_name varchar(20),
	entry_date char(8),
	update_date char(8),
	primary key(element_code)
);

/* #4 transaction order  master */
create table tm_trn_order
(
	wc				char(6),
	job_class_code char(3),
	job_pattern_no	char(1),
	trn_name varchar(20),
	entry_date char(8),
	update_date char(8),
		/* 00-09 */
		function_no_00 char(2),
		element_code_00		char(3),
		function_no_01 char(2),
		element_code_01		char(3),
		function_no_02 char(2),
		element_code_02		char(3),
		function_no_03 char(2),
		element_code_03		char(3),
		function_no_04 char(2),
		element_code_04		char(3),
		function_no_05 char(2),
		element_code_05		char(3),
		function_no_06 char(2),
		element_code_06		char(3),
		function_no_07 char(2),
		element_code_07		char(3),
		function_no_08 char(2),
		element_code_08		char(3),
		function_no_09 char(2),
		element_code_09		char(3),
		/* 10-19 */
		function_no_10 char(2),
		element_code_10		char(3),
		function_no_11 char(2),
		element_code_11		char(3),
		function_no_12 char(2),
		element_code_12		char(3),
		function_no_13 char(2),
		element_code_13		char(3),
		function_no_14 char(2),
		element_code_14		char(3),
		function_no_15 char(2),
		element_code_15		char(3),
		function_no_16 char(2),
		element_code_16		char(3),
		function_no_17 char(2),
		element_code_17		char(3),
		function_no_18 char(2),
		element_code_18		char(3),
		function_no_19 char(2),
		element_code_19		char(3),
		/* 20-29 */
		function_no_20 char(2),
		element_code_20		char(3),
		function_no_21 char(2),
		element_code_21		char(3),
		function_no_22 char(2),
		element_code_22		char(3),
		function_no_23 char(2),
		element_code_23		char(3),
		function_no_24 char(2),
		element_code_24		char(3),
		function_no_25 char(2),
		element_code_25		char(3),
		function_no_26 char(2),
		element_code_26		char(3),
		function_no_27 char(2),
		element_code_27		char(3),
		function_no_28 char(2),
		element_code_28		char(3),
		function_no_29 char(2),
		element_code_29		char(3),
		/* 30-39 */
		function_no_30 char(2),
		element_code_30		char(3),
		function_no_31 char(2),
		element_code_31		char(3),
		function_no_32 char(2),
		element_code_32		char(3),
		function_no_33 char(2),
		element_code_33		char(3),
		function_no_34 char(2),
		element_code_34		char(3),
		function_no_35 char(2),
		element_code_35		char(3),
		function_no_36 char(2),
		element_code_36		char(3),
		function_no_37 char(2),
		element_code_37		char(3),
		function_no_38 char(2),
		element_code_38		char(3),
		function_no_39 char(2),
		element_code_39		char(3),
	primary key(wc,job_class_code,job_pattern_no)
);

/* #5 machine master */
create table tm_machine
(
	wc			char(6),
	machine_no	 	char(3),
	machine_name 		varchar(20),
	plc_m			char(4),
	ip			varchar(20),					/*Max 2006-06-12*/
	entry_date 		char(8),
	update_date 		char(8),
	primary key(wc,machine_no)
);



CREATE TABLE tm_fraction_rack
(
  	fraction_rack_no 	char(10) ,
  	wc 		char(6),
  	plc_m 		char(4),
  	status_use 	char(1),
  	item 		char(30),
  	entry_date 	char(8),
	entry_user	char(25),
  	PRIMARY KEY (fraction_rack_no, wc)
);

CREATE TABLE tm_balance_rack
(
  	balance_rack_no 	char(10) ,
 	 wc 		char(6) ,
  	plc_m 		char(4),
  	status_use 	char(1),
  	item 		char(30),
  	entry_date 	char(8),
	entry_user	char(25),
  	PRIMARY KEY (balance_rack_no, wc)
);

create table TM_USER
(
	LOCATION_CD	char (3),
	USER_ID 	char(3),
	FIRST_NAME	char(12),
	LAST_NAME	char(12),
	ADMIN_SIGN	char(1),
	PW		char(10),
	ENTRY_DATE	char(8),
	PRIMARY KEY( LOCATION_CD ,USER_ID )
);


/*CREATE TABLE tm_postmachine
(
  	machine_no 	char(5) ,
  	wc 		char(6),
  	temperature 	float8,
 	time_use 	float8,
  	weight 		float8,

  	PRIMARY KEY (machine_no, wc)
); 
*/
CREATE TABLE tm_postmachine
(
	WC				char(6),
	MACHINE_NO		char(3),
	PATTERN_NO		char(1),
	MACHINE_NAME	varchar(30),
	TEMPERATURE		int4,
	TIME_USE		char(4),
	ENTRY_DATE		char(8),
	UPDATE_DATE		char(8),
	COMPOUND_NAME01	varchar(10),
	COMPOUND_NAME02	varchar(10),
	COMPOUND_NAME03	varchar(10),
	COMPOUND_NAME04	varchar(10),
	COMPOUND_NAME05	varchar(10),
	COMPOUND_NAME06	varchar(10),
	COMPOUND_NAME07	varchar(10),
	COMPOUND_NAME08	varchar(10),
	COMPOUND_NAME09	varchar(10),
	COMPOUND_NAME10	varchar(10),
	COMPOUND_NAME11	varchar(10),
	COMPOUND_NAME12	varchar(10),
	COMPOUND_NAME13	varchar(10),
	COMPOUND_NAME14	varchar(10),
	COMPOUND_NAME15	varchar(10),
	COMPOUND_NAME16	varchar(10),
	COMPOUND_NAME17	varchar(10),
	COMPOUND_NAME18	varchar(10),
	COMPOUND_NAME19	varchar(10),
	COMPOUND_NAME20	varchar(10),
	PRIMARY KEY (wc,machine_no,pattern_no)
); 


CREATE TABLE  tm_print_pack
(
	job_no		char(7) ,
 	suffix		int2 ,
  	machine_no	char(6),
	wc		char(6),
	LOCATION_CD	char (3),	
	lot_no		char(6),
	tag_no		char(5),
	status		char(1),
	entry_user	char(25),
	entry_date	char(8),
	update_date	char(8),
	
  	PRIMARY KEY (job_no,suffix,wc,machine_no)
);



CREATE TABLE  tm_print_box
(
	job_no		char(7) ,
 	suffix		int2 ,
  	machine_no	char(6),
	wc		char(6),
	LOCATION_CD	char (3),	
	lot_no		char(6),
	tag_no		char(5),
	status		char(1),
	entry_user	char(25),
	entry_date	char(8),
	update_date	char(8),
	  	
  	PRIMARY KEY (job_no,suffix,wc,machine_no)
);

CREATE TABLE  tm_control_lot
(
	item		char(30),
	plant		char(3),
	lot_type		char(1),
		  	
  	PRIMARY KEY (item,plant)
);


CREATE TABLE tm_workcenter
(
  	file_sign 		char(3),
  	active_sign	 char(1),
  	entry_date 	char(8),
  	update_date 	char(8),
  	update_time 	char(6),
 	 update_wsid 	char(25),
 	 update_pgm 	char(10),
 	 update_sign 	char(1),
  	wc char(6) 		NOT NULL,
  	wc_name 		varchar(20),
  CONSTRAINT tm_workcenter_pkey PRIMARY KEY (wc)
) 
