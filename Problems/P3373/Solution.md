## ���� �������mul_tag��add_tag

��ǵ�����: �ڸ��������t��sum֮���ñ�Ǽ�¼���µ������Ȼ���ټ����ݹ������������sum�����Ժ�query��update��Ҫ�ݹ鵽t������ʱ��
�ٸ��ݱ�Ǽ������������ȷ��sum���̶����Լ�����ȷ�ĵݹ顣  
��[�߶���1](https://www.luogu.org/problem/P3372)�У�ֻ��һ��update������һ�η�Χ�ڵ�ÿ��������һ��ֵ�����������t��sumʱ��tag(���)�������ӵ�ֵ��
�Ժ���Ҫ�ݹ�t������ʱ��t��������sum����tag\*�����ĳ���(���Գ�������Ϊÿ��������Ҫ����)���ɡ���ʱt��������sum�õ�����ȷ��ֵ����t�������ı����Ҫ��¼��
��Ϊt��������������û�и��¡�  
��[�߶���2](https://www.luogu.org/problem/P3373)�У����Խ�һ�η�Χ�ڵ�ÿ�������ӣ�Ҳ���Խ����ǳ���һ��ֵ������Ҫ�������mul_tag��add_tag
���������t��mul_tag��ʼ��Ϊ1��add_tag��ʼ��Ϊ0  
��t��������Ҫͨ��t�ı�Ǹ���ʱ��  
t��������sum = (t��������sum) \* mul_tag + add_tag \* (t�������ĳ���)  
����  
* �������t�ķ�Χ�ڵ�ÿ��������addʱ��  
  t��������sum��Ӧ������add \* (t�������ĳ���)��  
  �� (t��������sum) \* mul_tag + add_tag \* (t�������ĳ���)  + add \* (t�������ĳ���)  
		=  (t��������sum) \* mul_tag + (add_tag + add) \* (t�������ĳ���)  
  ��ôt��add_tagӦ�ø���Ϊadd_tag + add�������Ժ�ݹ�t������ǰ�Ϳ���ͨ����Ǽ������������ȷsum

* �������t�ķ�Χ�ڵ�ÿ��������mulʱ��  
   t��������sum��Ӧ�ó���mul��  
   �� ( (t��������sum) \* mul_tag + add_tag \* (t�������ĳ���) ) \* mul  
			= (t��������sum) \* mul_tag \* mul + add_tag \* mul \* (t�������ĳ���)
   ��ôt��add_tagӦ�ø���Ϊmul_tag \* mul��t��add_tagӦ�ø���Ϊadd_tag \* mul�������Ժ�ݹ�t������ǰ�Ϳ���ͨ����Ǽ������������ȷsum