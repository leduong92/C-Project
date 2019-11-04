/*
  TNC Production process control system
  create.sql
  dateabase create script for PostgreSQL
  2006.05.12 
*/

/* Master files from S/L*/
/* #1 Work Center Master */
create table tm_workcenter
(
	file_sign	char(3),
	active_sign	char(1),
	entry_date	char(8),
	update_date	char(8),
	update_time	char(6),
	update_wsid	char(25),
	update_pgm	char(10),
	update_sign	char(1),
	wc		char(6),
	wc_name	varchar(20),
	primary key(wc)
);

/* Data files from S/L*/
/* #2 mps_info */
create table tr_mps_info
(
	file_sign	char(3),
	active_sign	char(1),
	entry_date	char(8),
	update_date	char(8),
	update_time	char(6),
	update_wsid	char(25),
	update_pgm	char(10),
	update_sign	char(1),
	mps_num		integer,
	co_num		char(7),
	mps_org_qty	float8,
	mps_qty		float8,
	mps_bal		float8,
	mps_total_qty_pc	float8,
	primary key(mps_num)
);

/* #3 job_progress */
create table tr_job_progress
(
	file_sign	char(3),
	active_sign	char(1),
	entry_date	char(8),
	update_date	char(8),
	update_time	char(6),
	update_wsid	char(25),
	update_pgm	char(10),
	update_sign	char(1),
	data_type	char(3),
	job_no		char(7),
	suffix		smallint,
	item		char(30),
	oper_num_00	smallint,
	wc_00			char(6),
	check_sign_00	char(1),
	oper_num_01	smallint,
	wc_01			char(6),
	check_sign_01	char(1),
	oper_num_02	smallint,
	wc_02			char(6),
	check_sign_02	char(1),
	oper_num_03	smallint,
	wc_03			char(6),
	check_sign_03	char(1),
	oper_num_04	smallint,
	wc_04			char(6),
	check_sign_04	char(1),
	oper_num_05	smallint,
	wc_05			char(6),
	check_sign_05	char(1),
	oper_num_06	smallint,
	wc_06			char(6),
	check_sign_06	char(1),
	oper_num_07	smallint,
	wc_07			char(6),
	check_sign_07	char(1),
	oper_num_08	smallint,
	wc_08			char(6),
	check_sign_08	char(1),
	oper_num_09	smallint,
	wc_09			char(6),
	check_sign_09	char(1),
	oper_num_10	smallint,
	wc_10			char(6),
	check_sign_10	char(1),
	oper_num_11	smallint,
	wc_11			char(6),
	check_sign_11	char(1),
	oper_num_12	smallint,
	wc_12			char(6),
	check_sign_12	char(1),
	oper_num_13	smallint,
	wc_13			char(6),
	check_sign_13	char(1),
	oper_num_14	smallint,
	wc_14			char(6),
	check_sign_14	char(1),
	oper_num_15	smallint,
	wc_15			char(6),
	check_sign_15	char(1),
	oper_num_16	smallint,
	wc_16			char(6),
	check_sign_16	char(1),
	oper_num_17	smallint,
	wc_17			char(6),
	check_sign_17	char(1),
	oper_num_18	smallint,
	wc_18			char(6),
	check_sign_18	char(1),
	oper_num_19	smallint,
	wc_19			char(6),
	check_sign_19	char(1),
	primary key(job_no,suffix)
);

/* #4 curing job */
create table tr_curing_job
(
	file_sign	char(3),
	active_sign	char(1),
	entry_date	char(8),
	update_date	char(8),
	update_time	char(6),
	update_wsid	char(25),
	update_pgm	char(10),
	update_sign	char(1),
	job_no		char(7),
	suffix		smallint,
	total_tag_no	integer,
	item	char(30),
	mps_num		integer,
	parent_item	varchar(30),
	parent_desc	varchar(40),
	parent_job	char(7),
	parent_suffix	smallint,
	wc			char(6),
	machine		smallint,
	due_date	char(8),
	job_total_qty	integer,
	qty_tag			integer,
	obj_data_sign	char(4),
	product_code	char(10),
	defective_rate	float8,
	start_date		char(6),
	compound	char(30),
	
	cure_temp		float8,
	cure_time		float8,
	postcure_temp	float8,
	postcure_time	float8,

	tag_no		char(5),

	bom0_item 	varchar(30),
	bom0_desc	varchar(40),
  	bom0_qty 	float8,
	bom0_um 	char(3),
	bom1_item 	varchar(30),
	bom1_desc 	varchar(40),
	bom1_qty 	float8,
	bom1_um 	char(3),
	bom2_item 	varchar(30),
	bom2_desc 	varchar(40),
	bom2_qty 	float8,
	bom2_um 	char(3),
	bom3_item 	varchar(30),
	bom3_desc 	varchar(40),
	bom3_qty 	float8,
	bom3_um 	char(3),
	bom4_item 	varchar(30),
	bom4_desc 	varchar(40),
	bom4_qty 	float8,
	bom4_um 	char(3),
	bom5_item 	varchar(30),
	bom5_desc 	varchar(40),
	bom5_qty 	float8,
	bom5_um 	char(3),
	bom6_item 	varchar(30),
	bom6_desc 	varchar(40),
	bom6_qty 	float8,
	bom6_um 	char(3),
	bom7_item 	varchar(30),
	bom7_desc 	varchar(40),
	bom7_qty 	float8,
	bom7_um 	char(3),
	bom8_item 	varchar(30),
	bom8_desc 	varchar(40),
	bom8_qty 	float8,
	bom8_um 	char(3),
	bom9_item 	varchar(30),
	bom9_desc 	varchar(40),
	bom9_qty 	float8,
	bom9_um 		char(3),
	
	primary key(job_no,suffix)
);
alter table tr_curing_job add column cure_cav varchar(7);


/********* RUBBER JOBTAG SYTELINE TO PC *****************/
create table TR_RUBBER_JOB(
	file_sign	char(3),
	active_sign	char(1),
	entry_date	char(8),
	update_date	char(8),
	update_time	char(6),
	update_wsid	char(25),
	update_pgm	char(10),
	update_sign	char(1),
	job_no		char(7),
	suffix		smallint,
	total_tag_no	integer,
	item		char(30),
	qty_tag		float8,
	container_qty	float8,
	start_date	char(8),
	end_date	char(8),
	status		char(1),
	wc		char(6),
	description	char(40),
	parent_item	varchar(30),
	parent_desc	varchar(40),
	parent_job	char(7),
	parent_suffix	smallint,

	cure_date	char(8),
	bom0_item	varchar(30),
	bom0_desc	varchar(40),
	bom0_qty	float8,	
	bom0_um		char(3),
	
	bom1_item	varchar(30),
	bom1_desc	varchar(40),
	bom1_qty	float8,	
	bom1_um		char(3),

	bom2_item	varchar(30),
	bom2_desc	varchar(40),
	bom2_qty	float8,	
	bom2_um		char(3),
	
	bom3_item	varchar(30),
	bom3_desc	varchar(40),
	bom3_qty	float8,	
	bom3_um		char(3),
	
	PRIMARY KEY( JOB_NO,SUFFIX )
);


/* #6 metal job */
create table tr_metal_job
(
	file_sign	char(3),
	active_sign	char(1),
	entry_date	char(8),
	update_date	char(8),
	update_time	char(6),
	update_wsid	char(25),
	update_pgm	char(10),
	update_sign	char(1),
	job_no		char(7),
	suffix		smallint,
	total_tag_no	integer,
	item		char(30),
	qty_tag		float8,
	container_qty	float8,
	start_date	char(8),
	end_date	char(8),
	status		char(1),
	wc	char(6),
	parent_item	varchar(30),
	parent_desc	varchar(40),
	parent_job	char(7),
	parent_suffix	smallint,

	cure_date	char(8),
	bom0_item	varchar(30),
	bom0_desc	varchar(40),
	bom0_qty	float8,	
	bom0_um		char(3),
	
	bom1_item	varchar(30),
	bom1_desc	varchar(40),
	bom1_qty	float8,	
	bom1_um		char(3),

	bom2_item	varchar(30),
	bom2_desc	varchar(40),
	bom2_qty	float8,	
	bom2_um		char(3),
	
	bom3_item	varchar(30),
	bom3_desc	varchar(40),
	bom3_qty	float8,	
	bom3_um		char(3),


	
	primary key(job_no,suffix)
);

/* #7 spring job */
create table tr_spring_job
(
	file_sign	char(3),
	active_sign	char(1),
	entry_date	char(8),
	update_date	char(8),
	update_time	char(6),
	update_wsid	char(25),
	update_pgm	char(10),
	update_sign	char(1),
	job_no		char(7),
	suffix		smallint,
	total_tag_no	integer,
	item		char(30),
	qty_tag		float8,
	container_qty	float8,
	start_date	char(8),
	end_date	char(8),
	status		char(1),
	wc	char(6),
	parent_item	varchar(30),
	parent_desc	varchar(40),
	parent_job	char(7),
	parent_suffix	smallint,

	cure_date	char(8),
	bom0_item	varchar(30),
	bom0_desc	varchar(40),
	bom0_qty	float8,	
	bom0_um		char(3),
	
	bom1_item	varchar(30),
	bom1_desc	varchar(40),
	bom1_qty	float8,	
	bom1_um		char(3),

	bom2_item	varchar(30),
	bom2_desc	varchar(40),
	bom2_qty	float8,	
	bom2_um		char(3),
	
	bom3_item	varchar(30),
	bom3_desc	varchar(40),
	bom3_qty	float8,	
	bom3_um		char(3),
	
	primary key(job_no,suffix)
);

/* #8 asm_job */
/*
create table tr_assembly_job
(
	file_sign	char(3),
	active_sign	char(1),
	entry_date	char(8),
	update_date	char(8),
	update_time	char(6),
	update_wsid	char(25),
	update_pgm	char(10),
	update_sign	char(1),
	job_no		char(7),
	suffix		smallint,
	total_tag_no	integer,
	item		char(30),
	mps_num		integer,
	job_total_qty	integer,	
	qty_tag			integer,
	wc			char(6),
	description	varchar(40),
	parent_item	varchar(30),
	parent_desc	varchar(40),
	parent_job	char(7),
	parent_suffix	smallint,
	machine		smallint,
	due_date	char(8),
	obj_data_sign	char(4),
	product_code	char(10),
	defective_rate	float8,

		bom0_job_no		char(7),
		bom0_suffix		smallint,
		bom0_item		varchar(30),
		bom0_qty		float8,

		bom1_job_no		char(7),
		bom1_suffix		smallint,
		bom1_item		varchar(30),
		bom1_qty		float8,

		bom2_job_no		char(7),
		bom2_suffix		smallint,
		bom2_item		varchar(30),
		bom2_qty		float8,

		bom3_job_no		char(7),
		bom3_suffix		smallint,
		bom3_item		varchar(30),
		bom3_qty		float8,

		bom4_job_no		char(7),
		bom4_suffix		smallint,
		bom4_item		varchar(30),
		bom4_qty		float8,

		bom5_job_no		char(7),
		bom5_suffix		smallint,
		bom5_item		varchar(30),
		bom5_qty		float8,

		bom6_job_no		char(7),
		bom6_suffix		smallint,
		bom6_item		varchar(30),
		bom6_qty		float8,

		bom7_job_no		char(7),
		bom7_suffix		smallint,
		bom7_item		varchar(30),
		bom7_qty		float8,

		bom8_job_no		char(7),
		bom8_suffix		smallint,
		bom8_item		varchar(30),
		bom8_qty		float8,

		bom9_job_no		char(7),
		bom9_suffix		smallint,
		bom9_item		varchar(30),
		bom9_qty		float8,

		bom10_job_no		char(7),
		bom10_suffix		smallint,
		bom10_item		varchar(30),
		bom10_qty		float8,

		bom11_job_no		char(7),
		bom11_suffix		smallint,
		bom11_item		varchar(30),
		bom11_qty		float8,

		bom12_job_no		char(7),
		bom12_suffix		smallint,
		bom12_item		varchar(30),
		bom12_qty		float8,

		bom13_job_no		char(7),
		bom13_suffix		smallint,
		bom13_item		varchar(30),
		bom13_qty		float8,

		bom14_job_no		char(7),
		bom14_suffix		smallint,
		bom14_item		varchar(30),
		bom14_qty		float8,

		bom15_job_no		char(7),
		bom15_suffix		smallint,
		bom15_item		varchar(30),
		bom15_qty		float8,

		bom16_job_no		char(7),
		bom16_suffix		smallint,
		bom16_item		varchar(30),
		bom16_qty		float8,

		bom17_job_no		char(7),
		bom17_suffix		smallint,
		bom17_item		varchar(30),
		bom17_qty		float8,

		bom18_job_no		char(7),
		bom18_suffix		smallint,
		bom18_item		varchar(30),
		bom18_qty		float8,

		bom19_job_no		char(7),
		bom19_suffix		smallint,
		bom19_item		varchar(30),
		bom19_qty		float8,
	primary key(job_no,suffix)
);
*/
/* #8 label_info */
create table tr_label_info
(
	file_sign	char(3),
	active_sign	char(1),
	entry_date	char(8),
	update_date	char(8),
	update_time	char(6),
	update_wsid	char(25),
	update_pgm	char(10),
	update_sign	char(1),
	job_no		char(7),
	total_tag_no	integer,
	item		char(30),
	cust_num	char(7),
	cust_seq	smallint,
	cust_name	char(25),
	cust_item	char(30),
	packing_type	char(2),
	packing_qty		integer,
	box_type		char(2),
	box_qty			integer,
	jis				char(1),
	special_sign	char(2),
	packing_part	char(40),
	box_part1		char(40),
	box_part2		char(40),
	delivery_place	char(15),
	comment_packing	char(20),
	obj_data_sign	char(4),
	rack_no			char(12),
	print_packing_sign	char(1),
	nok_cust_code	char(5),
	nok_cust_item	char(40),
	nok_ins_place	char(4),
	nok_loc			char(15),
	nok_center		char(4),
	nok_p_koza		char(5),
	nok_b_koza		char(5),
	compound		char(30),
	primary key(job_no)
);

