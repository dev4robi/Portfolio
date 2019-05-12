package com.ghasix.manager;

import java.security.Key;
import java.util.Base64;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Map;

import javax.annotation.PostConstruct;
import javax.crypto.spec.SecretKeySpec;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.annotation.PropertySource;
import org.springframework.core.env.Environment;
import org.springframework.stereotype.Component;

import com.robi.util.CipherUtil;
import com.robi.util.JwtUtil;

import io.jsonwebtoken.ExpiredJwtException;
import io.jsonwebtoken.IncorrectClaimException;
import io.jsonwebtoken.MalformedJwtException;
import io.jsonwebtoken.MissingClaimException;
import io.jsonwebtoken.security.Keys;
import io.jsonwebtoken.security.SignatureException;

@Component
@PropertySource("classpath:config.properties")
public class UserJwtManager extends AbsManager {
    // [Class private constants]
    private final Logger logger = LoggerFactory.getLogger(UserJwtManager.class);
    
    // [Class private variables]
    private String              jwtSubject = null;      // JWT 제목
    private int                 jwtLifeMin = 0;         // JWT 유효 시간 (분)
    private Map<String, Object> jwtReqClaims = null;    // JWT 필수 Claims
    private Key                 jwtSignKey = null;      // JWT 해시 키 (SHA256 해시 서명 키)
    private SecretKeySpec       jwtKeySpec = null;      // JWT 암호화 키 (AES128 암호화 키)
    
    private Environment env;                            // 설정파일 클래스

    // [Constructor]
    public UserJwtManager(Environment env) {
        this.env = env;
    }
    
    // [Methods]
    @PostConstruct
    @Override // 매니저 초기화 함수
    public boolean init() {
        try {
            // Name and Version
            managerName     = "UserJwtManager";
            managerVersion  = "1.0.0";

            // JwtHeaderType
            jwtSubject = env.getProperty("userJwtMgr.jwtSubject");

            // JwtLifeMin
            jwtLifeMin = Integer.valueOf(env.getProperty("userJwtMgr.jwtLifeMin"));

            // JwtReqClaims
            jwtReqClaims = new HashMap<String, Object>();
            jwtReqClaims.put("sub", jwtSubject);

            // JwtHashKey
            byte[] jwtHashKeyByte = null;
            jwtHashKeyByte = env.getProperty("userJwtMgr.jwtHashKey").getBytes();

            if (jwtHashKeyByte.length != 32) { // JwtSecretKey값이 32Byte가 아니면, 패딩하거나 자름
                byte[] newJwtSecretKeyByte = new byte[32];
                System.arraycopy(jwtHashKeyByte, 0, newJwtSecretKeyByte, 0,
                        Math.min(jwtHashKeyByte.length, newJwtSecretKeyByte.length));
                jwtHashKeyByte = newJwtSecretKeyByte;
            }
    
            jwtSignKey = Keys.hmacShaKeyFor(jwtHashKeyByte);
    
            // JwtKeySpec
            byte[] aes128KeyBytes = new byte[16];
            byte[] aesKeyBytesFromEnv = env.getProperty("userJwtMgr.jwtAesKey").getBytes();
            System.arraycopy(aesKeyBytesFromEnv, 0, aes128KeyBytes, 0,
                    Math.min(aes128KeyBytes.length, aesKeyBytesFromEnv.length));
            jwtKeySpec = new SecretKeySpec(aes128KeyBytes, "AES");
            managerInitialized = true;
        }
        catch (Exception e) {
            logger.error("Exception whlie Manager Initialization!", e);
            managerInitialized = false;
        }
        
        return managerInitialized;
    }

    // UserId로 JWT 생성하여 문자열로 반환
    public String encodeUserJwt(String userId) {
        // 매니저 초기화 여부 검사
        if (!managerInitialized) {
            if (!init()) {
                logger.error("Manager initialization failed!");
                return null;
            }
        }

        // 파라미터 검사
        if (userId == null) {
            logger.error("Param check error! 'userId' is null!");
            return null;
        }

        // JWT Claims 생성
        HashMap<String, Object> jwtClaims = new HashMap<String, Object>();

        {
            // JWT 필수 Claim
            jwtClaims.putAll(jwtReqClaims);
        }
        
        {
            // 유저 ID ('userId')
            jwtClaims.put("userId", userId);
        }
        
        {
            // 토큰 발행시간 ('iat'), 토큰 만료시간 ('exp')
            Calendar curCal = Calendar.getInstance();
            long curTime = curCal.getTimeInMillis();
            jwtClaims.put("iat", curTime);
            
            curCal.add(Calendar.MINUTE, jwtLifeMin);
            long expTime = curCal.getTimeInMillis();
            jwtClaims.put("exp", expTime);
        }

        // JWT 생성
        String jwtStr = null;
        
        if ((jwtStr = JwtUtil.buildJwt(jwtClaims, jwtSignKey)) == null) {
            logger.error("'jwtStr'is null! BuildJwt fail!");
            return null;
        }

        // JWT 암호화
        byte[] encJwtBytes = null;
        
        if ((encJwtBytes = CipherUtil.encrypt(CipherUtil.AES_CBC_PKCS5, jwtStr.getBytes(), jwtKeySpec)) == null) {
            logger.error("'encJwtBytes'is null! JWT Encryption fail!");
            return null;
        }
        
        // JWT를 Base64로 변환
        String base64Jwt = null; 
        
        if ((base64Jwt = Base64.getEncoder().encodeToString(encJwtBytes)) == null) {
            logger.error("'base64Jwt'is null! Base64 Encoding fail!");
            return null;
        }

        // Base64문자 중 URL에서 사용이 제한적인 문자를 치환
        base64Jwt = base64Jwt.replace("/", "_");
        base64Jwt = base64Jwt.replace("=", "-");

        return base64Jwt;
    }

    // JWT String 디코딩하여 맵으로 반환
    public Map<String, Object> decodeUserJwt(String base64JwtStr)
            throws IllegalArgumentException, MalformedJwtException, ExpiredJwtException, 
            SignatureException, MissingClaimException, IncorrectClaimException {
        // 매니저 초기화 여부 검사
        if (!managerInitialized) {
            if (!init()) {
                logger.error("Manager initialization failed!");
                return null;
            }
        }

        // 파라미터 검사
        if (base64JwtStr == null) {
            logger.error("Param check error! 'base64JwtStr' is null!");
            return null;
        }
        
        // Base64문자 중 URL사용이 제한적인 문자를 원래대로 되돌림
        base64JwtStr = base64JwtStr.replace("_", "/");
        base64JwtStr = base64JwtStr.replace("-", "=");

        // Base64디코딩 수행
        byte[] base64Bytes = null;
        
        if ((base64Bytes = Base64.getDecoder().decode(base64JwtStr.getBytes())) == null) {
            logger.error("'base64Bytes' is null! Base64 Decoding fail! (base64JwtStr:" + base64JwtStr + ")");
            return null;
        }
        
        // 토큰 복호화 수행
        byte[] decJwtBytes = null;
        
        if ((decJwtBytes = CipherUtil.decrypt(CipherUtil.AES_CBC_PKCS5, base64Bytes, jwtKeySpec)) == null) {
            logger.error("'decJwtBytes' is null! JWT Decrypt fail!");
            return null;
        }
        
        // JWT 파싱 및 결과 반환
        String jwtStr = new String(decJwtBytes);
        Map<String, Object> rtMap = null;
        
        rtMap = JwtUtil.parseJwt(jwtStr, jwtReqClaims, jwtSignKey);
        return rtMap;
    }
}