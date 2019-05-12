package com.spring.yesorno.validator;

import org.springframework.validation.Errors;
import org.springframework.validation.ValidationUtils;
import org.springframework.validation.Validator;
import org.springframework.web.multipart.MultipartFile;

import com.spring.yesorno.command.VoteBoardWriteCmd;

public class VoteBoardWriteValidator implements Validator {

	@Override
	public boolean supports(Class<?> clazz) {
		return VoteBoardWriteValidator.class.isAssignableFrom(clazz);
	}

	@Override
	public void validate(Object target, Errors errors) {
		final Integer[] TitleLength = {2, 64};
		final Integer[] ContentLength = {2, 256};
		final Integer[] FileNameLength = {5, 128};
		final String[] ImgFileExt = {"jpg", "jpeg", "png", "bmp"};
		final long ThumbnailSizeLimit = 1048576; // 1MB
		VoteBoardWriteCmd cmd = (VoteBoardWriteCmd)target;
		final String boardTitle = cmd.getVoteBoardTitle();
		final String boardContent = cmd.getVoteBoardContent();
		

		// 타이틀 빈값 체크
		int errorSizeBefore = errors.getAllErrors().size();
		ValidationUtils.rejectIfEmptyOrWhitespace(errors, "voteBoardTitle", "error.required");
		int errorSizeAfter = errors.getAllErrors().size();
		if (errorSizeBefore != errorSizeAfter) {}
		// 타이틀 길이 (2~64)
		else if (boardTitle.length() < TitleLength[0] || boardTitle.length() > TitleLength[1]) {
			errors.rejectValue("voteBoardTitle", "error.length", new String[]{TitleLength[0].toString(), TitleLength[1].toString()}, null);
		}
		// 타이틀 통과
		else {}
		
		// 콘텐츠 빈값 체크
		errorSizeBefore = errors.getAllErrors().size();
		ValidationUtils.rejectIfEmptyOrWhitespace(errors, "voteBoardContent", "error.required");
		errorSizeAfter = errors.getAllErrors().size();
		if (errorSizeBefore != errorSizeAfter) {}
		// 콘텐츠 길이 (2~256)
		else if (boardContent.length() < ContentLength[0] || boardContent.length() > ContentLength[1]) {
			errors.rejectValue("voteBoardTitle", "error.length", new String[]{ContentLength[0].toString(), ContentLength[1].toString()}, null);
		}
		// 콘텐츠 통과
		else {}
		
		// 이미지 체크
		MultipartFile img = cmd.getVoteBoardThumbnailImage();
		if (img != null && img.getSize() > 0) {
			String fileName = img.getOriginalFilename();
			int fileNameLength = fileName.length();

			// 파일명 길이 체크
			if (fileNameLength < FileNameLength[0] || fileNameLength > FileNameLength[1]) {
				errors.rejectValue("voteBoardThumbnailImage", "fileNameLength.length",
								   new String[]{FileNameLength[0].toString(), FileNameLength[1].toString()}, null);
			}
			
			// 파일 확장자 체크
			final String fileExt = fileName.substring(fileName.lastIndexOf(".") + 1, fileNameLength);
			boolean extResult = false; 
			
			for (String ext : ImgFileExt) {
				if (fileExt.equals(ext)) {
					extResult = true;
					break;
				}
			}
			
			if (!extResult) {
				errors.rejectValue("voteBoardThumbnailImage", "error.fileExtNotSupported");
			}

			// 파일  크기 체크
			if (img.getSize() > ThumbnailSizeLimit) {
				errors.rejectValue("voteBoardThumbnailImage", "error.fileSizeOver");
			}
		}
		
		// 남은 인원, 일자 체크
		// ...
	}
}
