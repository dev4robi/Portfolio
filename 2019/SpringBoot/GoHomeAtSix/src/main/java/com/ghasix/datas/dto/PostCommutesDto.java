package com.ghasix.datas.dto;

import lombok.Data;

@Data
public class PostCommutesDto {
    
    private String commuteCompanyName;  // 출퇴근 회사명
    private long checkInTime;           // 출근 시간
    private long checkOutTime;          // 퇴근 시간
    private String memo;                // 메모
}