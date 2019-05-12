package com.ghasix.service;

import java.util.Map;

import com.ghasix.datas.result.ApiResult;
import com.ghasix.manager.UserJwtManager;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;

import io.jsonwebtoken.MalformedJwtException;
import io.jsonwebtoken.MissingClaimException;
import io.jsonwebtoken.ExpiredJwtException;
import io.jsonwebtoken.IncorrectClaimException;
import io.jsonwebtoken.security.SignatureException;

import lombok.AllArgsConstructor;

@Service
@AllArgsConstructor
public class UserJwtService {

    private final Logger logger = LoggerFactory.getLogger(UserJwtService.class);

    private UserJwtManager userJwtMgr;

    // [Methods]
    // userJwt를 디코딩하여 내부 정보를 Map<String, Object>형태로 반환
    public ApiResult getUserDataFromJwt(String userJwt) {
        if (userJwt == null) {
            logger.error("'userJwt' is null!");
            return ApiResult.make("00202"); // 회원토큰 값이 비었습니다.
        }

        Map<String, Object> userJwtMap = null;

        try {
            userJwtMap = userJwtMgr.decodeUserJwt(userJwt);
            
            if (userJwtMap == null) {
                return ApiResult.make("00201"); // 회원토큰 분석중 오류가 발생했습니다.
            }
        }
        catch (IllegalArgumentException e) {
            logger.error("'userJwt' is null or zero length! (userJwt:" + userJwt +")");
            return ApiResult.make("00202"); // 회원토큰 값이 비었습니다.
        }
        catch (MalformedJwtException e) {
            logger.error("'userJwt' format is NOT JWT! (userJwt:" + userJwt + ")");
            return ApiResult.make("00203"); // 회원토큰이 올바르지 않은 형식입니다.
        }
        catch (ExpiredJwtException e) {
            logger.error("'userJwt' is expired token! (userJwt:" + userJwt + ")");
            return ApiResult.make("00204"); // 회원토큰이 만료되었습니다.
        }
        catch (SignatureException e) {
            logger.error("'userJwt' has wrong sign! (userJwt:" + userJwt + ")");
            return ApiResult.make("00205"); // 변조된 회원 토큰입니다.
        }
        catch (MissingClaimException e) {
            logger.error("'userJwt' has missing vital claim! (userJwt:" + userJwt + ")");
            return ApiResult.make("00206"); // 회원토큰에서 필수값이 비었습니다.
        }
        catch (IncorrectClaimException e) {
            logger.error("'userJwt' has woring vital claim's value! (userJwt:" + userJwt + ")");
            return ApiResult.make("00207"); // 회원토큰에서 필수값이 올바르지 않습니다.
        }

        String userId = (String) userJwtMap.get("userId");

        logger.info("JWT decode success! (userId:" + userId + ")");
        return ApiResult.make("00000", "userId", userId);
    }
}