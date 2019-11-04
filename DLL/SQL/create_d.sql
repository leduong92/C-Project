/*
  TNC Production process control system
  create.sql
  dateabase create script for PostgreSQL
  2006.05.12 
*/

/* result files */
/* #1 job progress result */
create table td_job_progress
(
	data_type	char(3),
	job_no		char(7),
	suffix		smallint,
	item		char(30),
	time_stamp	char(14),
		oper_num_00	smallint,
		wc_00			char(6),
		check_sign_00	char(1),
		result_sign_00	char(1),
		
		oper_num_01	smallint,
		wc_01			char(6),
		check_sign_01	char(1),
		result_sign_01	char(1),
		
		oper_num_02	smallint,
		wc_02			char(6),
		check_sign_02	char(1),
		result_sign_02	char(1),
		
		oper_num_03	smallint,
		wc_03			char(6),
		check_sign_03	char(1),
		result_sign_03	char(1),
		
		oper_num_04	smallint,
		wc_04			char(6),
		check_sign_04	char(1),
		result_sign_04	char(1),
		
		oper_num_05	smallint,
		wc_05			char(6),
		check_sign_05	char(1),
		result_sign_05	char(1),
				
		oper_num_06	smallint,
		wc_06			char(6),
		check_sign_06	char(1),
		result_sign_06	char(1),
				
		oper_num_07	smallint,
		wc_07			char(6),
		check_sign_07	char(1),
		result_sign_07	char(1),
				
		oper_num_08	smallint,
		wc_08			char(6),
		check_sign_08	char(1),
		result_sign_08	char(1),
				
		oper_num_09	smallint,
		wc_09			char(6),
		check_sign_09	char(1),
		result_sign_09	char(1),
				
		oper_num_10	smallint,
		wc_10			char(6),
		check_sign_10	char(1),
		result_sign_10	char(1),
				
		oper_num_11	smallint,
		wc_11			char(6),
		check_sign_11	char(1),
		result_sign_11	char(1),
				
		oper_num_12	smallint,
		wc_12			char(6),
		check_sign_12	char(1),
		result_sign_12	char(1),
				
		oper_num_13	smallint,
		wc_13			char(6),
		check_sign_13	char(1),
		result_sign_13	char(1),
				
		oper_num_14	smallint,
		wc_14			char(6),
		check_sign_14	char(1),
		result_sign_14	char(1),
				
		oper_num_15	smallint,
		wc_15			char(6),
		check_sign_15	char(1),
		result_sign_15	char(1),
				
		oper_num_16	smallint,
		wc_16			char(6),
		check_sign_16	char(1),
		result_sign_16	char(1),
				
		oper_num_17	smallint,
		wc_17			char(6),
		check_sign_17	char(1),
		result_sign_17	char(1),
				
		oper_num_18	smallint,
		wc_18			char(6),
		check_sign_18	char(1),
		result_sign_18	char(1),
				
		oper_num_19	smallint,
		wc_19			char(6),
		check_sign_19	char(1),
		result_sign_19	char(1),
		
		inspection_0		char(10),
		Judge_0			char(1),		
		inspection_1		char(10),
		Judge_1			char(1),
		inspection_2		char(10),
		Judge_2			char(1),
		inspection_3		char(10),
		Judge_3			char(1),
		inspection_4		char(10),
		Judge_4			char(1),

	primary key(job_no,suffix)
);
/* #2 job auctual result */

create table td_job_result
(
	entry_date	char(8),
	entry_time	char(6),
	entry_user	char(25),
	job_no		char(7),
	suffix		smallint,
	wc		char(6),
	job_class_code	char(3),
	machine_no		char(3),
	dimension_no	smallint,
	element_no		char(3),
	data			char(15),
	primary key(entry_date,entry_time,job_no,suffix,wc,job_class_code,dimension_no)
);
create table td_rubber_fp_result
(
	entry_date	char(8),
	entry_time	char(6),
	entry_user	char(25),
	wc		char(6),
	machine_no		char(3),
	job_no		char(7),
	suffix		smallint,
	compound_no		char(14),
	status			char(1),
	primary key(entry_date,entry_time,wc,machine_no)
);
create index TD_BOX_INFO_INDEX2 on td_rubber_fp_result(
	wc,machine_no
);


/********* TRANSACTION COUNTER *****************/
create table TD_REC_COUNTER(
	TABLE_NAME	char(25),
	CURRENT_NO	integer,
	START_NO	integer,
	END_NO		integer,
	UPD_DATE	char(8),
	PRIMARY KEY( TABLE_NAME )
);

/***********      TABLE  Td_fraction_label     **************/
CREATE TABLE td_fraction_label
(
 	 fraction_label 	char(12) ,
 	 wc 		char(6) ,
 	 job_no 		char(7),
  	suffix 		int2,
  	qty 		int4,
  	item 		char(30),
  	fraction_rack_no 	char(10),
 	status_use 	char(1),
 	entry_date 	char(8),
	entry_user	char(25),
  	PRIMARY KEY (fraction_label, wc)
);


CREATE TABLE  td_balance_label
(
  	packing_label	char(12) ,
 	wc 		char(6) ,
  	job_no		char(7),
	suffix		int2,
	qty		int4,
	item 		char(30),
  	balance_rack_no	char(10),
	status_use 	char(1),
  	entry_date 	char(8),
	entry_user	char(25),
  	PRIMARY KEY  (packing_label,wc)
);

CREATE TABLE  td_packing_label
(
  	packing_label	char(12) ,
 	wc 		char(6) ,
  	job_no		char(7),
	suffix		int2,
	qty		int4,
	item 		char(30),
	lot_no		char(6),
	tag_no		char(5),
  	box_no		char(13),
	pack_date 	char(8),
	pack_time	char(6),
	pack_user	char(25),
	box_date 	char(8),
	box_time	char(6),
	box_user	char(25),
	machine_no char(6),
	status_box char(1),
	
 	
   	PRIMARY KEY  (packing_label,wc)
);


create table td_postcure_result
(
	entry_date	char(8),
	entry_time	char(6),
	entry_user	char(25),
	wc_curing	char(6),
	wc_post		char(6),
	machine_no	char(3),
	job_no		char(7),
	suffix		smallint,
	compound_no	char(14),
	group_time	char(14),
	post_temp	float8,
	post_time	float8,
	end_date	char(8),
	end_time	char(6),
	end_user	char(25),
	
	primary key(entry_date,entry_time,job_no,suffix)
);


// RP  BPK1
CREATE TABLE  td_rubber_stock
(
  	rack_no		char(6) ,
 	box_no		char(6) ,
  	location_cd	char(3),
	expiry_date	char(8),
	entry_date		char(8),
	entry_time		char(6),
  	batch_no		char(10),
	compound	char(15),
  	weight		float8,
	entry_user		char(30),
	status_use	char(1),
  	PRIMARY KEY  (rack_no,box_no,location_cd)
);

CREATE TABLE  td_jobbatch
(
  	job_no		char(7) ,
 	suffix		int2,
	user_id		char(6),
	compound1	char(14),
  	batch_no11	char(10),
	batch_no12	char(10),
	batch_no13	char(10),
	weight1		float8,
	compound2	char(14),
  	batch_no21	char(10),
	batch_no22	char(10),
	batch_no23	char(10),
	weight2		float8,
	PRIMARY KEY  (job_no,suffix)
);

