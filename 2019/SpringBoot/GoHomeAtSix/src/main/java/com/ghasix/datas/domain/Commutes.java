package com.ghasix.datas.domain;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Table;

import lombok.AccessLevel;
import lombok.Builder;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.ToString;

@Entity
@Table(name = "commutes")
@NoArgsConstructor(access = AccessLevel.PROTECTED)
@Getter
@ToString
public class Commutes {

    @Id @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id") // 고유 ID
    private Long id;

    @ManyToOne @JoinColumn(name = "own_user_id", nullable = false)
    private Users ownUserId;

    @Column(name = "commute_company_name", length = 64, nullable = false) // 출퇴근 회사 이름
    private String commuteCompanyName; 

    @Column(name = "check_in_time", nullable = false) // 출근시간
    private Long checkInTime;

    @Column(name = "check_out_time") // 퇴근시간
    private Long checkOutTime;

    @Column(name = "memo", length = 256) // 메모
    private String memo;

    @Builder
    public Commutes(Users ownUserId, String commuteCompanyName, Long checkInTime, Long checkOutTime, String memo) {
        this.ownUserId = ownUserId;
        this.commuteCompanyName = commuteCompanyName;
        this.checkInTime = checkInTime;
        this.checkOutTime = checkOutTime;
        this.memo = memo;
    }
}