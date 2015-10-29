#include "headers_and_defines.h"
#include <bitset>
struct test_instr {
	string name;
	uint32_t whole;
	uint32_t opcode;
    	uint32_t src1;
	uint32_t src1_data;
   	uint32_t src2; 
	uint32_t src2_data;
   	uint32_t src3;
	uint32_t src3_success_data;
	uint32_t pc_bef;
	uint32_t pc_aft;
    	uint32_t shift;
   	uint32_t function;
	uint32_t idata;
	int type; // R = 1, I = 2, J = 3 
	
	test_instr(uint32_t pc_diff, string name_in,  uint32_t src1_in, uint32_t src1_data_in, uint32_t src2_in, uint32_t src2_data_in, uint32_t src3_in, uint32_t src3_success_data_in, mips_cpu_h cpu_in, mips_mem_h mem_in ) {

		name = name_in;
		if (name_in == "addu") {
			function = 0x21;
			shift = 0;
			type = 1;
		} else if (name_in == "and") {
			function =  0x24;
			shift = 0;
			type = 1;
		} else if (name_in == "or") {
			function = 0x25;
			shift = 0;
			type = 1;
		} else if (name_in == "xor") {
			function =  0x26;
			shift = 0;
			type = 1;
		} else if (name_in == "sltu") {
			function = 0x2B;
			shift = 0;
			type = 1;
		} else if (name_in == "subu") {
			function =  0x23;
			shift = 0;
			type = 1;
		} else if (name_in == "sll") {
			function = 0x00;
			shift = 0;
			type = 1;
		} else if (name_in == "srl") {
			function =  0x02;
			shift = 0;
			type = 1;
		} else if (name_in == "sllv") {
			function = 0x04;
			shift = 0;
			type = 1;
		} else if (name_in == "srlv") {
			function = 0x06;
			shift = 0;
			type = 1;
		} else if (name_in == "sra") {
			function = 0x03;
			shift = 0;
			type = 1;
		} else if (name_in == "srav") {
			function = 0x07;
			shift = 0;
			type = 1;
		} else if (name_in == "add") {
			function = 0x20;
			shift = 0;
			type = 1;
		} else if (name_in == "sub") {
			function =  0x22;
			shift = 0;
			type = 1;
		} else if (name_in == "slt") {
			function = 0x2A;
			shift = 0;
			type = 1;
		} else if (name_in == "andi") {
			opcode = 0x0C;
			shift = 0;
			type = 1;
		} else if (name_in == "xori") {
			opcode = 0x0E;
			shift = 0;
			type = 2;
		} else if (name_in == "ori") {
			opcode = 0x0D;
			shift = 0;
			type = 1;
		} else if (name_in == "addiu") {
			opcode = 0x09;
			shift = 0;
			type = 1;
		} else if (name_in == "addi") {
			opcode = 0x08;
			shift = 0;
			type = 1;
		} else if (name_in == "sltiu") {
			opcode = 0x0B;
			shift = 0;
			type = 1;
		} else if (name_in == "slti") {
			opcode = 0x0A;
			shift = 0;
			type = 1;
		} else if (name_in == "lw") {
			opcode = 0x23;
			shift = 0;
			type = 1;
		} else if (name_in == "lui") {
			opcode = 0x0F;
			shift = 0;
			type = 1;
		} else if (name_in == "sw") {
			opcode = 0x2B;
			shift = 0;
			type = 1;
		} 


		if (type == 1) {
			
			opcode = 0;
			src1 = src1_in;
			src1_data = src1_data_in;
			src2 = src2_in;
			src2_data = src2_data_in;
			src3 = src3_in;
			src3_success_data = src3_success_data_in;


			mips_error e = mips_cpu_set_register(cpu_in, src1, src1_data);
			e = mips_cpu_set_register(cpu_in, src2, src2_data);
			e = mips_cpu_get_pc(cpu_in,&pc_bef);
			pc_aft = pc_bef + pc_diff;
			uint32_t whole =
				(src1 << 21) // srca = r4
				|
				(src2 << 16) // srcb = r5
				|
				(src3 << 11) // dst = r3
				|
				(shift << 6) // shift = 0
				|
				(function << 0);
    
			uint8_t buffer[4];
			buffer[0]=(whole>>24)&0xFF;
			buffer[1]=(whole>>16)&0xFF;
			buffer[2]=(whole>>8)&0xFF;
			buffer[3]=(whole>>0)&0xFF;

			e = mips_mem_write(
				mem_in,	        //!< Handle to target memory
				pc_bef,	            //!< Byte address to start transaction at
				4,	            //!< Number of bytes to transfer
				buffer	        //!< Receives the target bytes
			);
			
			if(e!=mips_Success){
				fprintf(stderr, "mips_cpu failed.\n");
				exit(1);
			}
		} else if (type == 2) {
			
			src1 = src1_in;
			src1_data = src1_data_in;
			idata = src2_data_in;
			src3 = src3_in;
			src3_success_data = src3_success_data_in;

			mips_error e = mips_cpu_set_register(cpu_in, src1, src1_data);
			e = mips_cpu_get_pc(cpu_in,&pc_bef);
			pc_aft = pc_bef + pc_diff;
			uint32_t whole =
				((opcode & 0x2F) << 26) 
				|
				((src1 & 0x1F) << 21) 
				|
				((src3 & 0x1F) << 16) 
				|
				((idata & 0xFFFF) << 0);
			uint8_t buffer[4];
			buffer[0]=(whole>>24)&0xFF;
			buffer[1]=(whole>>16)&0xFF;
			buffer[2]=(whole>>8)&0xFF;
			buffer[3]=(whole>>0)&0xFF;

			e = mips_mem_write(
				mem_in,	        //!< Handle to target memory
				pc_bef,	            //!< Byte address to start transaction at
				4,	            //!< Number of bytes to transfer
				buffer	        //!< Receives the target bytes
			);

			if(e!=mips_Success){
				fprintf(stderr, "mips_cpu failed.\n");
				exit(1);
			}
		}
	}
	

};

void r_type_test(test_instr instr, mips_cpu_h cpu_in) {
	
	int testId = mips_test_begin_test(instr.name.c_str());   
	
	uint32_t got;
	uint32_t pc_new;
	if (instr.name == "xori") {
		cout << instr.src3 << endl;
		cout << "0x" << hex <<  instr.src3_success_data << endl;
	}	
	
	mips_error e = mips_cpu_get_register(cpu_in, instr.src3, &got);
	e = mips_cpu_step(cpu_in);
	e = mips_cpu_get_pc(cpu_in,&pc_new);
	if(e!=mips_Success){
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}
	int passed;

	if (got == instr.src3_success_data && instr.pc_aft == pc_new) {
		passed = 1;
	}
	
    stringstream ss;
	ss << instr.name << "failed, with inputs " << instr.src1_data << " and  " << instr.src2_data << endl;
	
	mips_test_end_test(testId, passed, ss.str().c_str());
}

int main()
{
    mips_mem_h mem=mips_mem_create_ram(4096, 4);
    
    mips_cpu_h cpu=mips_cpu_create(mem);
    
    mips_error e = mips_cpu_set_debug_level(cpu, DEBUG_LEVEL, stderr);
    if(e != mips_Success){
        fprintf(stderr, "mips_cpu_set_debug_level : failed.\n");
        exit(1);
    }
    
    
    mips_test_begin_suite();
	
	//ADDU
	test_instr addu1(4, "addu", 10, 0xFFFFFFFF, 4, 0xFFFFFFFF, 31, 0xFFFFFFFE, cpu, mem);
	r_type_test(addu1, cpu);
	
	test_instr addu2(4, "addu", 0, 0x01, 2, 0x1111111, 3, 0x1111111, cpu, mem);
	r_type_test(addu2, cpu);
	
	test_instr addu3(4, "addu", 4, 0x5555555, 2, 0x5555555, 0, 0x00, cpu, mem);
	r_type_test(addu3, cpu);
	
	//AND
	test_instr and1(4, "and", 10, 0xF0F0F0F0, 4, 0x0000FFFF, 31, 0x0000F0F0, cpu, mem);
	r_type_test(and1, cpu);
	
	test_instr and2(4, "and", 0, 0x01, 2, 0x1111111, 3, 0x0, cpu, mem);
	r_type_test(and2, cpu);
	
	test_instr and3(4, "and", 4, 0x6666666, 2, 0x6666666, 0, 0x00, cpu, mem);
	r_type_test(and3, cpu);
	
	//OR
	test_instr or1(4, "or", 10, 0xF0F0F0F0, 4, 0x0000FFFF, 31, 0xF0F0FFFF, cpu, mem);
	r_type_test(or1, cpu);
	
	test_instr or2(4, "or", 0, 0xFFFFFFFF, 2, 0xF0F0F0F0, 3, 0xF0F0F0F0, cpu, mem);
	r_type_test(or2, cpu);
	
	test_instr or3(4, "or", 4, 0x6666666, 2, 0x6666666, 0, 0x00, cpu, mem);
	r_type_test(or3, cpu);
	
	//XOR
	test_instr xor1(4, "xor", 10, 0xF0F0F0F0, 4, 0x0000FFFF, 31, 0xf0f00f0f, cpu, mem);
	r_type_test(xor1, cpu);
	
	test_instr xor2(4, "xor", 0, 0xFFFFFFFF, 2, 0xFFFFFFFF, 3, 0xFFFFFFFF, cpu, mem);
	r_type_test(xor2, cpu);
	
	test_instr xor3(4, "xor", 4, 0x6666666, 2, 0x6666666, 0, 0x00, cpu, mem);
	r_type_test(xor3, cpu);

	
	test_instr xori1(4, "xori", 5, 0xF0F0F0F0, 0, 0x0000FFFF, 10, 0xf0f00f0f, cpu, mem);
	r_type_test(xori1, cpu);

	mips_test_end_suite();
    
    return 0;
}
