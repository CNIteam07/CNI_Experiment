package mynet;

import java.net.InetAddress;
import java.net.NetworkInterface;

public class Test {
        @SuppressWarnings("static-access")
		public static void main(String[] args) throws Exception {
            InetAddress ia=null;
            try {
                ia=ia.getLocalHost();
                String localname=ia.getHostName();
                String localip=ia.getHostAddress();
                System.out.println("��������"+ localname);
                System.out.println("����ip��"+localip);
            } catch (Exception e) {
                e.printStackTrace();
            }
            InetAddress ia1 = InetAddress.getLocalHost();//��ȡ����IP����
            System.out.println("MAC ."+getMACAddress(ia1));
        }
        //��ȡMAC��ַ�ķ���
        private static String getMACAddress(InetAddress ia)throws Exception{
            byte[] mac = NetworkInterface.getByInetAddress(ia).getHardwareAddress();//�������ӿڶ��󣨼������������õ�mac��ַ��mac��ַ������һ��byte�����С�
            StringBuffer sb = new StringBuffer();    //��������ǰ�mac��ַƴװ��String
            for(int i=0;i<mac.length;i++){
                if(i!=0){
                    sb.append("-");
                }
                String s = Integer.toHexString(mac[i] & 0xFF);   //mac[i] & 0xFF ��Ϊ�˰�byteת��Ϊ������
                System.err.println(s);
                sb.append(s.length()==1?0+s:s);
            }
            return sb.toString().toUpperCase(); //���ַ�������Сд��ĸ��Ϊ��д��Ϊ�����mac��ַ������
        }
}