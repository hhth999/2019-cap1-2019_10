﻿"""
Django settings for mysite project.

Generated by 'django-admin startproject' using Django 2.1.7.

For more information on this file, see
https://docs.djangoproject.com/en/2.1/topics/settings/

For the full list of settings and their values, see
https://docs.djangoproject.com/en/2.1/ref/settings/
"""

import os

# Build paths inside the project like this: os.path.join(BASE_DIR, ...)
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


# Quick-start development settings - unsuitable for production
# See https://docs.djangoproject.com/en/2.1/howto/deployment/checklist/

# SECURITY WARNING: keep the secret key used in production secret!
SECRET_KEY = 'k$z+he_b$)9l$hm6i-=!-#4des_@h!v1vj$7_x*%-u(j$(qml%'

# SECURITY WARNING: don't run with debug turned on in production!
DEBUG = True

# logging
LOGGING = {
    'version': 1,
    'disable_existing_loggers': False,

    'formatters': {
        'format1': {
            'format': '[%(asctime)s] %(levelname)s [%(name)s:%(lineno)s] %(message)s',
            'datefmt': '%d/%b/%Y %H:%M:%S'
        },
        'format2': {
            'format': '%(levelname)s %(message)s'
        },
    },


    'handlers': {
        'console': {
            'level': 'DEBUG',
            'class': 'logging.StreamHandler',
            'formatter': 'format2',
        }
    },

    'loggers': {
        'my_mood_music.django.request': {
            'handlers': ['console'],
            'propagate' : False,
            'level': 'DEBUG',
        },
    },

}

ALLOWED_HOSTS = [
    '127.0.0.1',
    'localhost',
    'MyMoodMusic.ap-northeast-2.elasticbeanstalk.com',
    'mymoodmusic-env-1.e93snf25rg.ap-northeast-2.elasticbeanstalk.com'
]


# Application definition

INSTALLED_APPS = [
    'my_mood_music.apps.MyMoodMusicConfig',
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
    'bookmark.apps.BookmarkConfig',
    'rest_framework',
    'rest_framework_swagger',
    'rest_framework.authtoken',

]


REST_FRAMEWORK = {
    'DEFAULT_PERMISSION_CLASSES': (
        'rest_framework.permissions.IsAuthenticated',
    ),
    'DEFAULT_AUTHENTICATION_CLASSES': (
        'rest_framework_jwt.authentication.JSONWebTokenAuthentication',
        'rest_framework.authentication.SessionAuthentication',
        'rest_framework.authentication.BasicAuthentication',
    ),
    'PAGINATE_BY': 10
}

SWAGGER_SETTING = {
    # 사용자가 Authorize 버튼을 클릭해 Token 등의 API키를 입력할 수 있음.
    'SECURITY_DEFINITIONS': {
        "api_key": {
            "type" : "apikey",
            "name" : "Authorization",
            "in" : "header"
        },
    },

    "LOGIN_URL" : "/admin/login/",
    "LOGOUT_URL" : "/admin/logout"

}

MIDDLEWARE = [
    'django.middleware.security.SecurityMiddleware',
    'django.contrib.sessions.middleware.SessionMiddleware',
    'django.middleware.common.CommonMiddleware',
    # 'django.middleware.csrf.CsrfViewMiddleware',

    'django.contrib.auth.middleware.AuthenticationMiddleware',
    'django.contrib.messages.middleware.MessageMiddleware',
    'django.middleware.clickjacking.XFrameOptionsMiddleware',
]
#    'django.middleware.csrf.CsrfViewMiddleware',
ROOT_URLCONF = 'mysite.urls'

TEMPLATES = [
    {
        'BACKEND': 'django.template.backends.django.DjangoTemplates',
        'DIRS': [os.path.join(BASE_DIR, 'templates')], # 수정
        'APP_DIRS': True,
        'OPTIONS': {
            'context_processors': [
                'django.template.context_processors.debug',
                'django.template.context_processors.request',
                'django.contrib.auth.context_processors.auth',
                'django.contrib.messages.context_processors.messages',
            ],
        },
    },
]

WSGI_APPLICATION = 'mysite.wsgi.application'


# Database
# https://docs.djangoproject.com/en/2.1/ref/settings/#databases

DATABASES = {
    'default': {
        'ENGINE': 'django.db.backends.mysql',
        'NAME': 'MyMoodMusic',
        'USER': 'MyMoodMusic',
        'PASSWORD': 'qwer1234',
        'HOST': 'mymoodmusic.cjcpvsk3fxnl.ap-northeast-2.rds.amazonaws.com',
        'PORT': '3306',
        'OPTIONS' : {
            'init_command': "SET sql_mode='STRICT_TRANS_TABLES'",
            # 'sql_mode' : 'traditional'
        },
    }
}


# Password validation
# https://docs.djangoproject.com/en/2.1/ref/settings/#auth-password-validators

AUTH_PASSWORD_VALIDATORS = [
    {
        'NAME': 'django.contrib.auth.password_validation.UserAttributeSimilarityValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation.MinimumLengthValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation.CommonPasswordValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation.NumericPasswordValidator',
    },
]


# Internationalization
# https://docs.djangoproject.com/en/2.1/topics/i18n/

LANGUAGE_CODE = 'ko-kr'

TIME_ZONE = 'Asia/Seoul'

USE_I18N = True

USE_L10N = True

USE_TZ = True

MEDIA_URL = '/media/'

MEDIA_ROOT = os.path.join(BASE_DIR, 'media')

# Static files (CSS, JavaScript, Images)
# https://docs.djangoproject.com/en/2.1/howto/static-files/

STATIC_URL = '/static/'

STATICFILES_DIRS = [os.path.join(BASE_DIR, 'static')]  # 추가
