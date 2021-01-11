#
#    Copyright 2020 Huawei Technologies Co., Ltd.
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#

FROM ubuntu:18.04

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
        apt-get -y install sudo dialog apt-utils
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections

#define all environment variable here
ENV THIRD_HOME=/usr/third-party
ENV CRANE_HOME=/usr/crane-framework
ENV UID=166
ENV GID=166
ENV USER_NAME=eguser
ENV GROUP_NAME=eggroup
ENV DEBIAN_FRONTEND=noninteractive
ENV ENV="/etc/profile"

# Set umask
RUN sed -i "s|umask 022|umask 027|g" /etc/profile

# Create the home directory.
RUN mkdir -p $THIRD_HOME \
  && mkdir -p $CRANE_HOME

# Create an app user so our program doesn't run as root.
RUN groupadd -r -g $GID $GROUP_NAME &&\
    useradd -r -u $UID -g $GID -d $CRANE_HOME -s /sbin/nologin -c "Docker image user" $USER_NAME

#set sudo right
RUN echo ${USER_NAME}' ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
RUN chmod 644 /etc/sudoers

COPY . $CRANE_HOME

WORKDIR $THIRD_HOME

RUN apt-get install -y ssh openssh-server git gcc g++ tar net-tools

RUN sh $CRANE_HOME/crane-env-pre.sh

WORKDIR $CRANE_HOME

#build crane-framework
RUN mkdir -p -m 755 build \
  && cd build \
  && cmake -DCRANE_BUILD_TESTS=true -DCMAKE_BUILD_TYPE=Release .. \
  && make \
  && chown -R $USER_NAME:$GROUP_NAME $CRANE_HOME \
  && chmod -R 777 /root

# Change to the app user.
USER $USER_NAME

CMD ["bash"]