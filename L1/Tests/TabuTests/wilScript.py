import pandas as pd
import scipy.stats as stats

altList = ['less', 'two-sided']#, 'greater']
zeroList = ['pratt', 'wilcox', 'zsplit']

data = pd.read_csv("ltm_data.csv", sep = ';')
for alt in altList:
	for zer in zeroList:
		print(alt, zer)
		s1 = stats.wilcoxon(data['LTM_7'], data['LTM_Sqrt'], alternative=alt, zero_method=zer)
		s2 = stats.wilcoxon(data['NO_LTM_7'], data['NO_LTM_Sqrt'], alternative=alt, zero_method=zer)
		s3 = stats.wilcoxon(data['LTM_7'], data['NO_LTM_7'], alternative=alt, zero_method=zer)
		s4 = stats.wilcoxon(data['LTM_Sqrt'], data['NO_LTM_Sqrt'], alternative=alt, zero_method=zer)

		print(s1)
		print(s2)
		print(s3)
		print(s4)
		#print(s1[0], s1[1], end = " ")
		#print(s2[0], s2[1], end = " ")
		#print(s3[0], s3[1], end = " ")
		#print(s4[0], s4[1], end = "\n")
