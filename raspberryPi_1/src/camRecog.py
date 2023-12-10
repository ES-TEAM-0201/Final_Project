import RPi.GPIO as GPIO
from PIL import Image
import cv2
import numpy as np
import os
import time
from picamera import PiCamera
from picamera.array import PiRGBArray
import sys

# pip install -> Image, pillow, numpy, opencv 필요
# sudo raspi-config -> interface -> camera -> enable 설정 *******
'''
# 초음파 센서 GPIO 핀 설정
TRIG_PIN = 15
ECHO_PIN = 18

# GPIO 초기화 설정
def init():
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(TRIG_PIN, GPIO.OUT)
    GPIO.setup(ECHO_PIN, GPIO.IN)
    
# 초음파 센서 거리 측정
def get_distance():
    GPIO.output(TRIG_PIN, GPIO.LOW)
    time.sleep(0.2) 

    GPIO.output(TRIG_PIN, GPIO.HIGH)
    time.sleep(0.00001)
    GPIO.output(TRIG_PIN, GPIO.LOW)

    while GPIO.input(ECHO_PIN) == GPIO.LOW:
        pulse_start = time.time()

    while GPIO.input(ECHO_PIN) == GPIO.HIGH:
        pulse_end = time.time()

    pulse_duration = pulse_end - pulse_start
    distance = pulse_duration * 34300 / 2   # 거리 측정
    return distance
'''

# 얼굴 인식 / 사용자 검출
def CamRecognition():
    recognizer = cv2.face.LBPHFaceRecognizer_create()
    recognizer.read('train/trainer.yml')
    cascadePath = '/home/pi/project/opencv/data/haarcascades/haarcascade_frontalface_default.xml'
    faceCascade = cv2.CascadeClassifier(cascadePath)
    font = cv2.FONT_HERSHEY_SIMPLEX

    # 사용자 ID 초기화
    id = 0

    # 사용자 이름 설정
    names = ['None', 'JY']

    # 카메라 캡쳐 초기화 (비디오 너비/높이 설정)
    cam = cv2.VideoCapture(0)
    cam.set(3, 640) 
    cam.set(4, 480) 

    # 얼굴 인식을 위한 최소 창 크기 설정
    minW = 0.1*cam.get(3)
    minH = 0.1*cam.get(4)

    # 검출 결과 저장 변수 선언
    pass_flag = 0

    # 검출 시작 시간 기록 -> 10초 이상 검출 시 종료
    start_time = time.time()

    print("\n=== 검출 프로그램 시작 ===\n")

    while True:
        ret, img =cam.read()
        img = cv2.flip(img, -1) # 수직으로 비디오 이미지 뒤집기 
        gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
        
        faces = faceCascade.detectMultiScale( 
            gray,
            scaleFactor = 1.2,
            minNeighbors = 5,
            minSize = (int(minW), int(minH)),
        )

        for(x,y,w,h) in faces:
            cv2.rectangle(img, (x,y), (x+w,y+h), (0,255,0), 2)
            id, confidence = recognizer.predict(gray[y:y+h,x:x+w])
            # 얼굴 검출 -> confidence 작을수록 높은 정확도
            if (confidence < 100):
                id = names[id]
                if (confidence <= 65): # 정확도 35% 이상이면 통과하도록 -> 정확도 높이려면 많은 training 이미지 사용
                    pass_flag = 1      # 검출 결과 일치
                confidence = "  {0}%".format(round(100 - confidence))
                
            else:
                id = "unknown"  # 저장되지 않은 사용자 -> 검출 결과 변경 X (10초 간 카메라 작동/검사)
                confidence = "  {0}%".format(round(100 - confidence))
            
            cv2.putText(img, str(id), (x+5,y-5), font, 1, (255,255,255), 2)
            cv2.putText(img, str(confidence), (x+5,y+h-5), font, 1, (255,255,0), 1)  
        
        cv2.imshow('camera', img)   

        # 현재 시간과 시작 시간을 비교하여 10초가 지났으면 종료
        elapsed_time = time.time() - start_time
        if elapsed_time >= 10:  # 10초 이상 검출 시
            pass_flag = -1      # 검출 결과 불일치 시, 검출 결과 저장 변수 변경 후 검출 종료
            break

        if pass_flag == 1:      # 검출 결과 일치 시, 검출 종료
            break

    # 검출 정리 및 종료
    print("\n===== 검출 결과 확인 =====")
    cam.release()
    cv2.destroyAllWindows()

    return pass_flag

# 이미지 및 라벨 데이터 얻기 (학습용)
def getImagesAndLabels(path, detector):
    imagePaths = [os.path.join(path,f) for f in os.listdir(path)]     
    faceSamples=[]
    ids = []
    for imagePath in imagePaths:
        PIL_img = Image.open(imagePath).convert('L')
        img_numpy = np.array(PIL_img,'uint8')
        id = int(os.path.split(imagePath)[-1].split(".")[1])
        faces = detector.detectMultiScale(img_numpy)
        for (x,y,w,h) in faces:
            faceSamples.append(img_numpy[y:y+h,x:x+w])
            ids.append(id)
    return faceSamples,ids

# 얼굴 이미지 학습
def Camtrain():
    # 얼굴 이미지 경로
    path = 'data'
    recognizer = cv2.face.LBPHFaceRecognizer_create()
    detector = cv2.CascadeClassifier('/home/pi/project/opencv/data/haarcascades/haarcascade_frontalface_default.xml');

    print ("\n=== 얼굴 이미지 학습 중 ===")
    faces,ids = getImagesAndLabels(path, detector)
    recognizer.train(faces, np.array(ids))
    # Save the model into trainer/trainer.yml
    recognizer.write('train/trainer.yml') # recognizer.save() worked on Mac, but not on Pi
    # Print the numer of faces trained and end program
    print("\n=== [INFO] {0} 번 사용자 훈련 완료 ===".format(len(np.unique(ids))))


# 얼굴 이미지 얻기
def Camdataset():
    # 비디오 너비/높이 설정
    cam = cv2.VideoCapture(0)
    cam.set(3, 640)
    cam.set(4, 480)
    face_detector = cv2.CascadeClassifier('/home/pi/project/opencv/data/haarcascades/haarcascade_frontalface_default.xml')

    # 사용자 아이디 (숫자) 입력
    face_id = input('\n 사용자 ID 입력 (숫자) ==>  ')
    print("\n=== 얼굴 캡처 초기화. 카메라를 보고 기다리세요 ===")

    # 각 사용자 얼굴 수집 횟수 초기화
    count = 0

    # 얼굴 이미지 수집
    while(True):
        ret, img = cam.read()
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        faces = face_detector.detectMultiScale(gray, 1.3, 5)
        for (x,y,w,h) in faces:
            cv2.rectangle(img, (x,y), (x+w,y+h), (255,0,0), 2)     
            count += 1
            # 캡처 이미지 data 폴더에 저장
            cv2.imwrite("data/User." + str(face_id) + '.' + str(count) + ".jpg", gray[y:y+h,x:x+w])
            cv2.imshow('image', img)
        if count >= 30: # 30개 얼굴 이미지 캡처 후 종료  ->  갯수 많을수록 정확도 향상
            break

    # 수집 정리 및 종료
    print("\n=== 프로그램 정리 및 종료 ===")
    cam.release()
    cv2.destroyAllWindows()


'''
def disCamRecognitionResult():
    try:
        while True:
            distance = get_distance()
            print(f"Distance: {distance:.2f} mm")
            time.sleep(0.2)
            if distance <= 100: # 일정 거리 안, 카메라 작동
                break

        pass_flag_value = CamRecognition() # 얼굴 검출 진행 및 결과 가져오기
        
        if pass_flag_value == -1:
            print("\n도둑 침입\n")
        elif pass_flag_value == 1:
            print("\n사용자 확인\n")
    
    except KeyboardInterrupt:
        GPIO.cleanup()
'''

# main에서는 얼굴 이미지 수집 및 학습이 완료되었다는 가정 하에, 얼굴 검출 및 결과 출력 CamRecognition()만 진행
# 얼굴 이미지 수집 및 학습 하려면 -> Camdataset() -> Camtrain() 과정 따로 필요
def main():
    #init()
    pass_flag_value = 99
    result = 0
    try:
        pass_flag_value = CamRecognition() # 얼굴 검출 진행 및 결과 가져오기
        
        if pass_flag_value == -1:
            print("\n도둑 침입\n")
            result = 0
            with open('example.txt', 'w') as file:
                file.write('1')
                
        elif pass_flag_value == 1:
            with open('example.txt', 'w') as file:
                file.write('0')
    
    
    except KeyboardInterrupt:
        GPIO.cleanup()

if __name__ == "__main__":
    main()
