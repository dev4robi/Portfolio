package com.ghasix.service;

import java.util.Map;

import com.ghasix.datas.domain.Users;
import com.ghasix.datas.domain.UsersRepository;
import com.ghasix.datas.enums.UsersStatus;
import com.ghasix.datas.result.ApiResult;
import com.robi.util.StringUtil;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;

import lombok.AllArgsConstructor;

@Service
@AllArgsConstructor
public class UsersService implements IUsersService {

    private final Logger logger = LoggerFactory.getLogger(UsersService.class);

    private UserJwtService userJwtSvc;
    private UsersRepository userRepo;

    // 회원 존재여부, 서비스 접근유효성등 검사
    public ApiResult checkUserStatus(String userJwt) {
        ApiResult jwtSvcResult = userJwtSvc.getUserDataFromJwt(userJwt);

        if (jwtSvcResult == null) {
            logger.error("'jwtSvcResult' is null!");
            return jwtSvcResult;
        }

        if (jwtSvcResult.checkResultCodeSuccess() == false) {
            logger.error("'jwtSvcResult's result code is NOT success!");
            return jwtSvcResult;
        }

        String email = (String) jwtSvcResult.getResultData("userId");
        Users selectedUser = null;

        try { // JPA - Select
            selectedUser = userRepo.findByEmail(email);
        }
        catch (Exception e) {
            logger.error("JPA Select Exception!", e);
            return ApiResult.make("10101"); // 회원 DB조회중 오류가 발생했습니다.
        }

        if (selectedUser == null) {
            logger.info("Fail to find user. (email:" + email + ")");
            return ApiResult.make("10001"); // 회원 정보가 존재하지 않습니다.
        }

        UsersStatus status = selectedUser.getStatus();

        if (!status.equals(UsersStatus.NORMAL)) {
            if (status.equals(UsersStatus.SLEEPING)) {
                logger.info("User found but, status is '" + status.getValue() + "'. (email:" + email + ")");
                return ApiResult.make("10004"); // 휴면중인 회원입니다.
            }
            else if (status.equals(UsersStatus.BLACKLIST)) {
                logger.info("User found but, status is '" + status.getValue() + "'. (email:" + email + ")");
                return ApiResult.make("10005"); // 블랙리스트 회원입니다.
            }
            else if (status.equals(UsersStatus.DEREGISTERED)) {
                logger.info("User found but, status is '" + status.getValue() + "'. (email:" + email + ")");
                return ApiResult.make("10006"); // 탈퇴한 회원입니다.
            }
            else {
                logger.info("User found but, status is 'Undefined'. (email:" + email + ")");
                return ApiResult.make("10003"); // 회원 상태값이 미정의된 값입니다.
            }
        }

        Long accessibleTime = selectedUser.getAccessibleTime();

        if (accessibleTime != null && System.currentTimeMillis() < accessibleTime) {
            logger.info("User found but, accessibleTime NOT reached. (accessibleTime:" + accessibleTime + ")");
            return ApiResult.make("10002"); // 아직 사용할 수 없는 계정입니다.
        }

        logger.info("User found and authorized! (selectedUser:" + selectedUser.toString() + ")");
        return ApiResult.make("00000", "selectedUser", selectedUser);
    }

    // 회원 추가
    public ApiResult insertUser(String email, String name) {
        if (email == null || name == null) {
            logger.error("'email' or 'name' is null! (email:" + email + ", name:" + name + ")");
            return ApiResult.make("00101"); // 필수 인자값이 비었습니다.
        }

        email = email.trim();
        name = name.trim();

        if (StringUtil.isEmail(email) == false) {
            logger.error("'email' is NOT correct! (email:" + email + ")");
            return ApiResult.make("00103"); // 올바른 이메일 형식이 아닙니다.
        }

        if (name.length() == 0) {
            logger.error("'name's length is zero!");
            return ApiResult.make("00105"); // 이름이 너무 짧습니다.
        }

        Users selectedUser = null;

        try { // JPA - Select
            selectedUser = userRepo.findByEmail(email);
        }
        catch (Exception e) {
            logger.error("JPA Select Exception!", e);
            return ApiResult.make("10101"); // 회원 DB조회중 오류가 발생했습니다.
        }

        if (selectedUser != null) {
            logger.error("'selectedUser' is NOT null! email duplicated! (email:" + email + ")");
            return ApiResult.make("00104"); // 사용중인 이메일입니다.
        }

        try { // JPA - Insert
            long curTime = System.currentTimeMillis();
            Users insertedUser = Users.builder().email(email)
                                                .name(name)
                                                .accessLevel(1)
                                                .status(UsersStatus.NORMAL)
                                                .joinTime(curTime)
                                                .lastLoginTime(0L)
                                                .accessibleTime(curTime)
                                                .build();

            if (userRepo.save(insertedUser) == null) {
                logger.error("'.save()' returns null!");
                throw new Exception();
            }
        }
        catch (Exception e) {
            logger.error("JPA Insert Exception!", e);
            return ApiResult.make("10102"); // 회원 DB추가중 오류가 발생했습니다.
        }

        logger.info("New users inserted! (email:" + email + ")");
        return ApiResult.make();
    }
}