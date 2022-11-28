server {
        listen 80;
        listen [::]:80;

        root /var/www;
        index index.html index.htm index.nginx-debian.html;

        server_name lab4.com www.lab4.com;

        location / {
                try_files $uri $uri/ =404;
        }
}
