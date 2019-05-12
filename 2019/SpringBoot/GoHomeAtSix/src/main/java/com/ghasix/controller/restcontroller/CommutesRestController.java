package com.ghasix.controller.restcontroller;

import java.util.Map;

import com.ghasix.datas.dto.PostCommutesDto;
import com.ghasix.datas.dto.PutCommutesDto;
import com.ghasix.datas.result.ApiResult;
import com.ghasix.service.CommutesService;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestHeader;
import org.springframework.web.bind.annotation.RestController;

import lombok.AllArgsConstructor;

@RestController
@AllArgsConstructor
public class CommutesRestController {

    private final Logger logger = LoggerFactory.getLogger(CommutesRestController.class);

    CommutesService commutesSvc;

    @GetMapping("/commutes/time/all") // 출퇴근 기록 전체 조회
    public ApiResult getCommutesTimeAll(
        @RequestHeader("userJwt") String userJwt) {
        return null;
    }

    @GetMapping("/commutes/{id}") // 특정 출퇴근 기록 조회
    public ApiResult getCommutesById(
        @PathVariable("id") long id,
        @RequestHeader("userJwt") String userJwt) {
        return null;
    }

    @GetMapping("/commutes/time/{beginTime}-{endTime}") // 출퇴근 기록 범위 조회
    public ApiResult getCommutesTimeBetween(
        @PathVariable("beginTime") long beginTime,
        @PathVariable("endTime") long endTime,
        @RequestHeader("userJwt") String userJwt) {
        return null;
    }

    @PostMapping("/commutes") // 출퇴근 기록 추가
    public ApiResult postCommutes(
        @RequestHeader("userJwt") String userJwt,
        @RequestBody PostCommutesDto postCommutesDto)
    {
        return commutesSvc.insertCommutes(userJwt, postCommutesDto);
    }

    @PutMapping("/commutes/{id}") // 특정 출퇴근 기록 수정
    public ApiResult putCommutes(
        @PathVariable("id") long id,
        @RequestHeader("userJwt") String userJwt,
        @RequestBody PutCommutesDto putCommutesDto) {
        return null;
    }

    @DeleteMapping("/commutes/{id}") // 특정 출퇴근 기록 삭제
    public ApiResult deleteCommutes(@PathVariable("id") long id,
        @RequestHeader("userJwt") String userJwt) {
        return null;
    }
}