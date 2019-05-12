package com.ghasix.service;

import java.util.Map;

import com.ghasix.datas.domain.Commutes;
import com.ghasix.datas.domain.CommutesRepository;
import com.ghasix.datas.domain.Users;
import com.ghasix.datas.dto.PostCommutesDto;
import com.ghasix.datas.dto.PutCommutesDto;
import com.ghasix.datas.result.ApiResult;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;

import lombok.AllArgsConstructor;

@Service
@AllArgsConstructor
public class CommutesService implements ICommutesService {

    private final Logger logger = LoggerFactory.getLogger(CommutesService.class);

    private UsersService usersSvc;
    private CommutesRepository commutesRepo;

    @Override
    public ApiResult selectCommutesAll(String userJwt) {
        return null;
    }

    @Override
    public ApiResult selectCommutesById(String userJwt, long commuteId) {
        return null;
    }

    @Override
    public ApiResult selectCommutesByTime(String userJwt, long beginTime, long EndTime) {
        return null;
    }

    @Override
    public ApiResult insertCommutes(String userJwt, PostCommutesDto postCommutesDto) {
        if (userJwt == null) {
            logger.error("'userJwt' is null!");
            return ApiResult.make("00202"); // 회원토큰 값이 비었습니다.
        }

        if (postCommutesDto == null) {
            logger.error("'postCommutesDto' is null!");
            return ApiResult.make("00107"); // 필수 인자값이 비었습니다.
        }

        ApiResult checkUserSvcResult = usersSvc.checkUserStatus(userJwt);

        if (checkUserSvcResult.checkResultCodeSuccess() == false) {
            logger.error("'checkUserSvcResult's response code is FAIL!");
            return checkUserSvcResult;
        }

        Users ownUser = (Users) checkUserSvcResult.getResultData("selectedUser");
        
        if (ownUser == null) {
            logger.error("'ownUser' is null!");
            return ApiResult.make("00102"); // 서버에서 값 획득에 실패했습니다.
        }

        // JPA - Insert
        Commutes insertCommutes = null;

        try {
            insertCommutes = Commutes.builder().ownUserId(ownUser)
                                               .commuteCompanyName(postCommutesDto.getCommuteCompanyName())
                                               .checkInTime(postCommutesDto.getCheckInTime())
                                               .checkOutTime(postCommutesDto.getCheckInTime())
                                               .memo(postCommutesDto.getMemo())
                                               .build();
            
            if (commutesRepo.save(insertCommutes) == null) {
                logger.error("'.save()' returns null!");
                throw new Exception();
            }
        }
        catch (Exception e) {
            logger.error("JPA Insert Exception!", e);
            return ApiResult.make("20102"); // 출퇴근기록 DB추가중 오류가 발생했습니다.
        }

        logger.info("Insert new commutes SUCCESS! (insertCommutes:" + insertCommutes.toString() + ")");
        return ApiResult.make();
    }

    @Override
    public ApiResult updateCommutes(String userJwt, PutCommutesDto putCommutesDto) {
        return null;
    }

    @Override
    public ApiResult deleteCommutes(String userJwt) {
        return null;
    }
}