# Currently  based upon the open sourcce image
FROM zerodiv/hhvm

# We take 3 arguments
ARG USER_NAME
ARG USER_ID
ARG USER_GROUP_ID

# remove any existing groups with the users group id, allowing us to add one for them
RUN getent group $USER_GROUP_ID | cut -d: -f1 | xargs -n 1 delgroup

# add a group for the user 
RUN addgroup --gid $USER_GROUP_ID $USER_NAME

# add the user that owns the image
RUN adduser --disabled-password --gecos '' --uid $USER_ID --gid $USER_GROUP_ID $USER_NAME

# copy over our service startup scripts and our sql for initializing the databases
COPY ./bin/service /tmp/service
COPY ./tests/sql /var/source/tests/sql

# update the image and install sudo
RUN apt-get update && apt-get install -y sudo

# add a entry to sudoers for our user
RUN echo "$USER_NAME ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

# stand up mysql and postgres and intialize the databases used for testing.
RUN /tmp/service/mysql.sh && /tmp/service/init/mysql.sh
RUN /tmp/service/postgres.sh && /tmp/service/init/postgres.sh

# pin the default user to the user owning the image
USER $USER_NAME
