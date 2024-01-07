# Write-up python_freak

A little write-up of the task from the Ping-CTF 2023, RE python!

## First look

I am posting the task below in its original form. As you can see it is a coded payload, so I started analysis by unpacking it.

```python
from base64 import b64decode
exec(b64decode('ZXhlYyhiNjRkZWNvZGUoJ2FXMXdiM0owSUhONWN3cGxlR1ZqS0dJMk5HUmxZMjlrWlNnbldsaG9iRmw1YUdsT2FsSnJXbGRPZGxwSFZXOUtNazE2WWtod1RXSnJOWE5hUldoTFlrWnJlbFp1YkdwTmJYZ3lXVzB4TkdOSFNsaGlSRUpNVWtWV00xUlZVa0prTURGRVlUQjBZVmRIYUhOWFdHeHZZVlUxY1ZWdGRHRldNRFV5VjJ0a1ZtSXdiM2hqUm14b1VqTm9ZVnBXWkc5alJsSjBZMFpPYUUxWVFsbFdSelZoV1ZaSmVGWnVXbFJoYTFvelZqRmtSMU5IVmtkalIzaFhUVzFvTTFaclkzaGtNVlY1VWxob2FsSnRhRlpaYlhoM1lVWldjVkpzWkd0aVJUVXhXVEJqTlZaRk1VVlNibkJYVmpOQ1RGbHJXbUZXYkZwVlVteGFUbFp1UWtWV1YzUmhaREZPU0ZScVdsSmlWM2h3V1d4YVNtVkdXbkZUYWxKVlRWVndSMVF4Vm1GaFZUQjVaVVpvVmsxR2NHaGFSVnBYVmxaT2MxUnRjR2hsYkZvMFZteGpNVmxXV2tkWGFscFNZV3MxWVZadGVGcE5SbFp5VjJ0a1dGSlViRmRhVlZwaFZHc3hjMU5xUmxkTmJtaDJXWHBHVG1WSFRrbFViVVpPVFc1b2QxWlVRbUZaVjBsNFYxaGtXR0V6VW5CV2JURlRVMnhzY2xadVpGVk5hMVkxV2xWV2MxbFdXbGhWYmxwYVZteHdZVnBFU2s5VFZrNXpZVVprYUdWc1draFdNblJUVTJzeFYySkdaRmRYUjNoWVdWaHdSMVV4VWxaWGEzQnNWbTVDU1ZwVmFFOVdSMFkyVm01c1dtRXhjRWhXVjNONFl6Rk9kV0pHVm1oTmJFbDZWa1pTUzFNeVRYbFVhMlJXWWxkb1dGVnFRWGhPYkZwSFdrUlNWMkY2VmtoV1J6VkhWa2RLUjFOdVNsWk5SbkF6Vld4YWEyTnNjRWxqUlRWT1VrVkpNbFpxU25kVE1XUklVMnRrVkZkSVFtRlpiR2h2VFRGYVZsWlVRbXROYkVwSVZXMTRTMVJ0U2taalJGWllWa1Z2TUZsNlNsZGpNVkp6VjJ4V2FFMHhTblpXYlhoclRrZFJlR05GVmxSaVdFSlFWVzF6TVdWR1ZYbGxSMFpvVmpCd01GcFZhSE5YYlZaeVUyNXdWbUV4Y0ZCYVJtUlBVbTFPU0dGSGJGZFNNMmQ0Vm1wR2EwMUhSWGhUV0d4V1lUSlNjRlV3V2t0V01XeHpXa2M1VkUxV2NGZFdiWE0xWVVVeGNsWnFXbFpOYm1neldWZHplRk5IVmtkU2JGSk9UVEJLTmxkWE1IaFNNbEpHVDFab1QxWnJTbFJXYm5CeVRWWmFkR1ZIT1ZWTlJGWlpWVzAxVjFsV1RrZFhiR3hhWVRGVmVGcEZXbGRPYkVweVZHeE9VMkpyU2twWGJGWlhUVVpSZUZSclpGZFdSVXBYVkZjMVExUkdXbkphUms1VVZqQndTbFV5TVRCV01VbDVZVVV4VjFKdFVUQldWRXBYVTBaYWMyRkhkR3hpUm5CV1ZrWmFWMVl3TUhoV1dIQnFVbnBzVlZSVlVrZGxiRnBJWTBWa1ZtRjZSa2RXYkZKSFZtc3hWMk5GVWxaaE1YQXpWVEJrVjFKck1WZFZiRkpUVFZWc05GWnRjRXRsYXpWWFZtdGFVRlpXY0hCVVZFbzBWMFpWZDFkdFJtdFNiRnBXVlRKNFQyRnRSalpXYm1oWFZucFdhRlpyV21GT2JFcFpZMFp3VjJKR2EzZFdWM1JoVWpGa1JrOVdXbXhTYkhCUFZGZDRTMDFXV1hsbFJ6bHFUVlV4TkZrd2FFOVpWVEI1WlVoQ1ZrMUdjR2haYWtaYVpERmtjbVJIYkZkaVZrcExWMWQwVjJReFVYbFNXR3hzVWxkNGFGWnNaRk5rYkd4WFYyeHdhMDFyTlVwVk1uaHZWakpHTmxaclpGaGhNVXBMVkZaYWMxWXhXbGxpUm1Sb1pXdGFWRlpHVm1GVGJWWnpZa2hTYkZOSFVsaFZiVFZEWlZaUmVHRkZUbFpOVlRWSlZsZHdSMVl3TVhWaFJrSlZWa1ZGTVZWdE1VcGxiVXBIVjIxc1dGSnJXVEZXYkZwVFV6RlZlVlJ1U2s5V1JuQldXVmQwZDJOc2JISmFSRkpVWWtoQ1JsVXhVa2RYYkZwelVtcE9XbUp1UWxSV2EyUkhZMjFPUjFkc2NGZGlWa3B2VmxkMFlXUXhUa2hXYTFwc1VtdEtWRnBYTVd0T2JGcElUbGhrVkUxVlZqUldWM1JoVmxkRmQyTkhPVmRpVkVVd1drUkdWbVZIUmtaUFZsSnBVbXh3U0ZaclkzaE5SMFpJVm01T2FsSkdjR0ZVVlZwYVRVWldjbGRyWkZoU01IQkpWVzE0UzFSc1NYcFVhbFpYVmtWYVdGbFVRVEZUUmtweVlVWlNhVkl5YUZaWFYzaHZWVEZzVjFkc1ZsSmlSVFZQV1d4YVMyVnNiSEpXYlVab1lYcEdTVnBWV2t0V2JGcEdZMFpTV21WcldsQlpNVnAzVG14T2MxVnNaR2hOU0VKSFZtcEtNR0V3TlVkVVdHUlBWbGRvYUZWcVNtOVhSbEpYV2tSU2ExWnNSak5XYlRWaFlrWmFjMk5GYkZwaE1VcFlXVlZrUzJNeVNYcGlSbVJPVm14d1dWWkdXbFpPVmtwMFUxaHdhVkpzY0U5VVZsWnlaVVprVjFWck9WZGhla0l6Vkd4U1lWWkhTa2RYYkZaVlZteGFURlZzV210amJIQkZVVzE0VjJKWVVUSldWbVI2WlVkRmVWSllaRTlYUlhCaFdWUkdZVTFzVW5KWGJscHJUV3R3U0ZadGN6RlZNREZ5VFZSYVYxSjZWak5WZWtaTFVqRlNXVnBHUWxSU1dFSlNWbTF3UzFVeFdsZFZia1pWWWxoU2NsWnRNVk5sYkZwSVRVaG9WMUpVUmxkVVZXaDNXVlpLUm1FemFGaFdiSEJUV2xkNFQyTnRUa2RVYXpWWFltdEZlVlpxUm05a01VMTRWMjVPYWxKR2NHRmFWM014VkRGc1ZWRlVSbFZXYkhCWFZtMXpOV0ZGTVhKalJXUmFZbTVDUkZsVVJtRlRSMFpHWlVaV1YwMHlhRmxYYTFwclZqRmtTRlZxV2xkaVJrcFBWbXhhV21Wc1dsVlVibVJwVFZVeE0xUldhRTlXTWtwMFlVYzVWMkpHV2xkVVZscFdaVVphZEdSRk9WTk5SbkJLVmxSS05HSXhXa2hUYkd4V1lsVmFXRlpyVmt0V1JsSlZVMnQwYWsxck5VaFdWM014VmpGYVNHVkVUbGRXTTFKMldWUktVMVl4Y0VsVWJGWnBWa1phVmxaR1ZtdFZNV3hYWWtaa1lWSnNjSEpaYTFaWFRsWmFXRTVWZEZWTlJFWlpWMnBPZDFkdFNsbGhSRTVYWVRKU1RGWnNaRWRTYlZKR1RsZHNVMkV6UVhwV2JYaFhWakZSZVZKWWJGUmhNWEJ6Vld4YWQyTkdjRmhOVnpsVlVteEtXVnBWV2s5aGJGcFpXa1JPVjFKNlJYZFpWbHBMVjFaV2RWRnRSbE5TVjNRMFZqRmFZVlp0Vm5OV2JrcHNVbXMxY0Zscll6Vk9iR1JZVFZSU2EwMVdXa2hWYlRWTFlURktjbGRzVGxWV00yaE1XbGQ0VjA1c1NuVlViV3hwVWxSRk1sZFhkRmRUTVZKelYyeHNWbGRIZUZkV2ExWjNWa1pzTmxKck5XeFNNSEJLVjJ0YVUxWXhXWGhUYlRsWVlUSk9ORlpFU2s5U01VNXlZa1pvYUdFelFscFdiWEJQVlRKU1YxcEdaRmhpV0VKUFZXMDFRazFzYkZaV2FrSlZZbFZ3U0ZZeU5WZFdNa3BaVVd0U1lWWXphRkJXYlhoclkxWk9kR1JGTldoaVJuQjJWbTE0VTFReFJYaFZhMlJYWVRGd1ZsbHNhRk5qUmxaellVWk9XbFp0VW5oVlYzQkRWREZhY21ORmNGaGhNbWhZVmpCa1IwNXRSalpTYkZwc1lURndVVlpXVWt0VE1XUkhWR3hzYWxKc1NsaFVWM04zWkRGYVZWTnVaRlJOVm5Bd1ZrZDBiMkZXU2xaT1dFWldZV3RLTTFwRVJsZGpiVVpJWkVkMFYySldTalpXYTFwdllqRmtSMU51VG1wU1JuQldXV3hvVGsxV2EzZFdXR2hVVmpCYVNGbHJXbUZVYlVwelUxaHdWMkpZUWxCYVJFcFRZMnMxV1ZOdGVGTmlSbkJWVjFkNFlWTXhUWGhXYmtwV1lUQTFjRmxyYUVObGJGcElaRWhrYUdKRmNGaFZNbmhQVjJzeGNWWnVTbUZTYkhCeVdYcEdVMlJYU2tkVmJHUlhZVEZzTmxacVNuZFRNVTE0Vkd0a1ZHSnNXbGRaYTJoRFkwWmFkR1JJWkd0aVJsWTBWbXhTVTJGdFJqWmlSbXhWVm14YVdGbFdWWGhrUmxKVlVXeGFUbUpyU2sxV2JYUnJWVEZhYzFWdVVrOVdNbWhZVkZaYVdrMXNaRmRXYlhSVVRWVXhOVlp0TlZkV2JVcFpWV3hhVlZaNlFURlVhMXByWXpGYVZWSnNUbGRpU0VKaFZqSjBhbVZHV25KTlZXaHJUVEpvV0ZSWGNFTk5NVkp6V2taa1YxWnRkekpXUjNNeFlVZFdjMUpZWkZkV1JYQXpWWHBHWVZOR1ZuSmlSbWhvVFZWd2IxWlhjRTlVYlZaelZXeGFXbVZzU2xCVmJYTXhaVVpWZVdWSFJtaE5WWEI2Vlcxd1UxWlhSbkpUYWs1VllrWndlVnBXVlRGV2JWSklaVWRzYUdWc1ZqUldiRkpLVFZkTmVWSnVTazlYUlhCd1ZXMTBZVlV4VWxobFJYUnNZa1p3UjFZeWVIZFVNVnB5VFZSU1YxSjZWa3hXYkdSR1pEQTFTV0pHV2xkU1dFSlZWMWh3UjFSdFZsaFZhMmhQVm14d1ZGbFljRmRTVm1SWlkwVmthazFYVWtkVVZscHZZVlpKZDFkc1VsVldWbkJvV2taYVVtUXhXblZYYlhoWFlraENXbFl5ZEdGV01rWldUVmhTVm1GNlJsZFpWRVpoVjBaU2MxZHNUbXRTTURWSVZUSnpOVlV4V2tkWGEyUlhZVEZ3ZGxsVVJrOWpNVTUxVW14YWFWWkdXbFpXUmxwWFV6RktWMXBJU21GU1JrcFdWbTE0ZDFkV1pISldhbEpwVW14d1YxWXhVa3RXTVVvMlZtcE9WMUl6VGpSV2JYTTFWakZhYzFWc1pGTmhNMEpTVm0weE1GbFdXbkpOVm1SVlYwZG9WbGx0ZEhkaFJsSldZVVpPYTAxV1dsaFpWV2hyVm14S2NtTkZhRnBoTVVwWVZteGFXbVZzUm5WU2JVWlhZa1pXTkZkVVJtRlNNVnBYVm01S1YySkdjRmxWYkdNMVRteGtXRTFVVW10TlZscElWVzE0WVdGV1NsZFhhemxXWVd0d1QxUlZXbFpsVjA1R1pFWmtWMDFFUlRKV2JYaHZVekZXUjFkWWNGSlhSM2hYVm10V2QxWkdiRFpTYXpsUFlrWktlRlpITVhOaFZrNUdVMnhhV0dKR2NGUlpha1pPWkRBeFdWUnNXbWhoTTBKWVZrWmplRlV5VVhoWGJGWlRZbGhDVDFacVJtRmxiR3QzVjI1T1YxSXdXbmxWTW5Rd1Ztc3hTRlZVUWxWV00yZ3pWV3BLUjFKdFRraFNhelZZVWxWc00xWXllRmRoTWtsNFlrWm9WMkpzU2xGV2JuQlhWbFphY1ZOcVVteFdiSEJKV2tWVk5WUnRTa1pXYWxaWFZucFdURlpFUmtwbFZsWjFWbXhrVjFadVFsRldWbEpIWXpKT2RGVnJXbWhTTTJoVVZGY3hiMlF4V25SalJVNXJUV3MxZWxkcldtdFdWMFY0VjI1S1ZWWXpRbGhaVlZwaFYxZE9SazlXVmxkaE1WbDZWbXhrZDFNeVJYbFRhMmhzVWpCYVlWcFhkRXRsYkZwV1ZtcENUMkpWY0VwV1YzaExWR3N4YzFOcVJsZFNSVXBZV2tjeFYxWXhaSE5pUms1b1lraENkMVpHWTNoVWF6RkhWbGhzYWxKWVVsaFdha0poWld4YVNHUkZPV2hpUlhBeFZWYzFhMVp0UlhsaFNFcGhVbXh3Y2xreWN6VlRWMDVIVld4a1YyRXhiRFpXYTFKSFZqSk5lRk5zWkZOaVIyaG9WV3BDWVZkR1VsVlRiVFZPWWtac05WcFZWVFZWTVZsNFVtcFdWV0pIVWtoV2JYTjRaRlpHY2sxV1pGZFdiSEJSVjFaU1MxTXhTblJXYTJocVVqTkNUMVZxVG05Vk1WcFhWV3QwVmsxck5UQldSelZYVmxkS2RWRnNWbGRpUmxWNFdrZDRjMDVzU25WVWJHaFRWa1ZhV2xkWGRHdFNNVlYzVFZWa1dHSnNXbFpVVlZwaFRXeHdWMWR0UmxSV2JGb3hWVmN4TUZSdFNsWk5TR1JYVmtWd00xVjZSbUZXTWtsNlkwZHdWRkp1UW05V1JsWnZWVEZPVjJOR1dtRlNlbXh2Vm0xNGMwMHhhM2RoUlhSWVVteHdlVlJzVm1GWlZrcEdZVE5vV0Zac2NGTmFSRUV4VmpGR2RHRkdaRmRoTW5jeVZqRmplRTVIVVhoVmExcHJVa1p3WVZwWGN6RlVNV3haWTBjMVRsSnNWalZaTUZZd1ZqSktWazFVVWxkU2VsWklXVlJCZUdOV1duUmhSbVJYVWxWd1RWWnFRbGROUlRWWVZHcGFWMkpYYUhCVmJGWjNVbXhaZVdSSGRFOVNiRXBJVmxkMGMxbFdUa2RYYkd4YVlURlZlRlJzV2xkT2JFcHlWR3hPVTJKclNrcFhiRlpXWlVaWmVGcEZXbFJoZWxaWVdWUkdkMVl4VWxkWGJFcHNWakZHTmxkclpIZFViRWw2WVVWa1YxWkZXblpWVkVaUFl6RktXV05HVm1sV1JscFNWbGQ0YWs1Vk5WZFhiRlpWWWxWYVQxbHJWbGRPVmxwWVRsVjBWVTFFUm5wWmFrNTNWMjFLV1dGRVRsZFdWbFV4Vm0weFUxSXhTbk5UYXpWcFVsaEJNbFp0ZEZkWlZteFlVbXhrVmxkSVFsbFpWRVpMWVVaV2NWUnRPVlZXYlhnd1ZHeGtNR0V4V25SVmJteFZUVlpLU0ZaRVNsZGtSbEp4Vm14a1UxSlZiRE5XYkZwaFVtMVdWMVp1VmxWaVJrcFlWbTE0ZG1Wc1duSldiVVpXVFZad2VWUnNXbXRoUmtwelUyNUtWVlo2UmxSV1JFWnpWakZ3UlZGdGJHbFNNMmhaVmpKMFlXUXhiRmhXYmxKb1VsaG9WbFpxVG05WFJuQldWMjVrVjFJd05ESldiVEZ2VlRBeFdWRnJXbGROYmxKeVZHdGFVMU5HVWxsaVJrSlRVbXR3VmxkV1pEQmtNVmw0V2toR1UyRjZiSEZaYkZwTFVqRlNjMWRzWkdoU1ZFWXdWbGQ0UzFkc1duTlhhMmhWVmtWYVVGWnRlSGRTTVhCSVkwVTFhR1ZyVlhoV2JURTBZVEpHY2s1VlpGVmlhMXBWV1d0YWQyTldWWGRYYkdSc1ZteEdORmRyYUd0VWJVcEdWbXBXVjFaNlZreFdSRXBYWTJ4T2NWTnNWazVTYkc5NVYyeGFZVmxXWkZkV2JrNVdZbGhTVDFSWE5XOVViRnB6Vld0MFRsSXhTbmxVYkZKaFYyc3dlV1ZHVG1GV2VrWjJWa1ZhVTFkSFZrWlBWVFZwVm10d05WWnJZM2hOUjBaWFUyeHNWbUZzY0dGWmJUVkRaR3hXTmxGWWFHcE5WbHA0VlZjeGIxWXhXbGRpZWtKWFZrVmFhRlZVUmt0U01rNUdZVWR3YkdGNlZtaFdSbEpEVXpGU1YxZHNWbFJoYTBweVdWaHdjMUl4YkhGVWJrNVhUVlpzTlZaWGVGZFpWa3BHWTBoR1ZXRXhjRkJaTVZwUFl6RmtkRkpyTlU1aWJXY3hWakZvZDFFeVRYaFdhMXByVWtad2NGVXdWbUZpTVZwelZteHdhMDFXVmpSV2JYaFBWVEpLVms1VlpHRlNWbkJVV1ZWa1MyUkdWbkZTYkdST1ltMW9VVlpHVWtKT1ZrcDBVMWh3YVZKdFVsbFZiRkpUVGxaYVYxVnJkRlpOYXpWNVZHeFdZV0ZXVGtkalJsWlZWbFpLZGxwV1dsSmxSbkJKVjIxMGFWSldXVEZXYkdNeFZERmFTRlpzVm1oTk0wSmhXV3hvYjAweFduSmFSV1JUWWxaS1NGVnRlRXRVYlVwR1kwUmFWMWRJUWt4WmFrcEhWbXM1VmxkdGJFNU5ibWhaVmxSQ1ZrMVhVWGhqUldoUFZqTlNjbFp0TVZObFZtdzJWRzFHV0ZKcmNIbFViR2h6VjBkR2NsZHJVbFppUjFKVFdsWmFjMDVzVm5OVmJXaG9UVEJLVjFZeWRHcE5WMDE1Vlc1S1RsTkhVbkpWYlhoTFZURnNkR1JGZEdoU2JYaFlWbTE0VDFSdFNsZFRibkJhWVRKb1RGbFhNVWRrUmxKWlkwWldWMDB4UmpOV1ZFbzBVekZPVjJORmFFOVdWR3h6V1d4Vk1FMVdXbkpYYkU1cFRWVXhNMVJXYUU5V01rcHpZMGhLV2xaRlJYaFpNbmh6WTJ4YWNWRnRlR2xXVnpoNVYxWldWMDFHVW5SVGEyUlVWMGRTVjFaclZuWmtNV3gwWlVkMGFrMVlRa1pXVnpFMFlWWk9SbE5zV2xkV2JIQnhWR3hhVTJNeFpISlhiV2hPVFRCS1ZsWnRlR0ZUYlZaelZWaGtWMkpyTlZkV2JYUlhUbFphV0U1VmRHaGlWVm93V1ZWU1UxWXhTalpXYm5CYVRXNW9NbHBWWkU5VFZsWnpVV3hvVTAweWFFMVdiWGhYVmpGUmVWSlliRlJoTVhCeVZXcE9RMkZHVWxoa1IwWllWbXhLZVZkclpIZFVNVXB5VGxob1ZrMXVhRmhaVlZwSFRtMUtSbFpzYUdoTmJFVjNWbGh3UWsxSFZuTmFTRVpVWWtkb1dGVnFSa2RPYkdSWVRWUlNhMDFWTlVoV01uaFhZVVpLV1ZWck9WcGhNWEJvVkZkNGMxWXhjRWhqUlRWVFlYcEZNRlpyWXpGVE1rWnlUVlZvYkZKV1dtRldiR1J2VlVaWmVXVkhSazlpUlhBeFZWZDRiMkZGTVhOVGEyaFhUV3BXTTFscVJrNWtNREZaVkd4a2FHRXdjSHBXYWtKclZUSlNWMVZZWkZkaWJIQnpWbTE0ZDFOc1ZYbGpSbVJvVFVSR01GWkdhSGRXVjBwMVVXdDRWMDF1YUZSVmJYaFRZMnhhZEdSRk5XaGlSbkIyVm0xNFUxUXhUWGhXYTJoVFlrWndWMWxYZEhkamJHeHlZVVZPYTFadFVuaFZNblJyVjBaS2RGVnNiRmhYU0VKVVZtdGtSMk50VGtkYVJuQk9WakpvTmxaV1pIcE9WbGw0VW01T1ZtSkZjRmxWYWtaTFZFWmFXR05GWkZwV01WcDZXV3RTWVZaSFNrZGpSMmhoVm5wR1QxcFhlRk5YUjFKSVpFWldVMkpHY0VoV2EyTjRUVWRHVjFOc2JHaFRSVnBoV2xkMFMyVnNXbGRYYTFwc1ZteGFNVlV5Y3pWaFIwcFlXbnBLVjFaRldsaFdSRXBLWlZaU1dXRkhiRk5pVjJoUVZtMXdTMDVIUmtkV2JrWlNZa2RTY0ZsWWNITlNNV3h4Vkc1T1YwMVhVa2RXTVZKSFdWWktSbU5JUmxWaE1YQlFXVEZhVDJNeFpIUmpSbVJPVFVWc05WWnFTbmRSTURGSFZHdG9WMkV5YUc5VmFrbzBWMFpTVm1GRlRteFdiR3cwVmpKek5WVXhXWGhTYWxaVllrZFNTRlp0TVZkamJHUnpVbXh3VjJKR2NGVldSbU40VlRKTmVWTnJiRmRpV0ZKd1dXdG9RMkZHV2toalJUbG9UV3hLV0ZkcmFFOVdWMFp6VTJ4V1ZWWldTblphVmxwU1pVWnJlbUZHVGs1aVJXOTRWakowYjFZeFpFaFNhbHBwVWxaS1ZsbHROVU5qVm5CWFYydDBhbFl3V2tkYVJWVXhZVlpLYzJKNlFsaGlSbkJ5V2tSQmQyVldUblZVYlhCVFlUQndWMWRXVWs5Vk1WcFhWV3hhVm1FeVVsZFZiWGgzVTJ4V1dFMVVRbGRXYTNCWFZHeFdUMWxXV25OWFdHUlhZVEZ3U0ZZeFdrOWpiVTVIVjJ4T1YxWkdXalZXTVZwdlpESkZlRlZzYUZaaWJGcFpXV3RvUTFWR1VsVlNibHBzVm14c05WcFZWbXRoTWtZMllrVmtXbUZyTlhwV2FrWmFaVlpXYzJGR2NFNU5NRXBvVmtkMFlWUXlUbGRVYms1WFlsVmFUMWxyYUVOU2JGbDVaRWQwVGxKdGVGaFdWM1J6V1ZaT1IxZHNiRnBoTVZWNFZHeGFWMDVzU25KVWJFNVRZbXRLU2xkc1ZsWmxSbGw0V2tWYVZHRjZWbGhaVkVaM1ZqRlNWMWR0ZEZOTlZrcDRWa2N4UjFSc1NrWlhhazVZVm14S1RGbFVTa2RTTVdSWllVZHNVMlZzV2xKV1YzaHFUbFUxVjFkdVNtaFNhelZ3Vm0xMFYwNVdXbGhPVlhSWVVqQldObGRxVG5kWGJVcFpZVVJPVjJFeVVsQlpNakZUVWpGS2MxTnJOV2xTV0VFeFZtdGtNRlV4UlhsVldHaFdZVEpTVjFsclduZFZSbHB6V2tSU1UySkdTbmxYYTJRd1ZsZEtSbFpxVGxaTmFsWlFWakl4Um1WWFJrZFJiSEJvVFZad05sWnNXbUZTYlZaWFZtNVdWR0pZUW05WmJGcEhUbFprY2xwRVVtcGhla0kxVmxab2MxUXhXWGRYYkZwaFZucEZNRlpHV21GVFYwcEpZMFU1VjAxSVFsaFdhMlIzVkRKS1IxZHNiRlppUmxwaFZtdFdjazFHVW5KWGF6bHFWbXMxZWxadE1YZFdiVlowWlVWYVYwMXVVbkpVYTFwVFVqSktTVlp0UmxSU2JIQmFWMWQwVmsxVk5WZFdhbHBUWVRBMVZGVnROVU5UVm10NlkwVTVXR0Y2UmpCV1YzTjRWMnhhYzFkc1VsVldSWEJNV1hwS1QxSXhWblJqUm1SWVVtdHdWMVl4V2xOVU1VVjRWV3RrVjJFeGNGWlpiR2hUWTBaV2MxcEVVbE5OVjNjeVZWYzFUMVpzV25KV2FsWlhWbnBXVUZac1drcGtNRFZWVkd4a1UxWnNjRkZXVjNoclZXMVdWazVXYUZOaVJuQlBWRlZvUTFSR1dsZGhTR1JTVFd0d2VsWlhlR3RXVjBweVkwaEdWbUpIYUhaVmJGcFhWakpHU0U5V1ZsZGhNVmw2VmxkNGIyRXhXbGRYV0hCU1lURktXVmxzYUU1bFJtdDRXa1ZhYkdKRldubFpNRnBQVkd4a1JsSnFWbGROVmtwTVdYcEdTMWRHVmxsaVJUbFhUVVp3V0ZaR1pIZFNNVTE0Vm01R1ZHRnNTazlXYlhNeFYwWldXR1JHVGxwV2EydzJWa1pvYTFkSFNsVldiV2hWVmpOb2NsWnRNVWRTVmxKMFlrZHNWRkpWY0ZOV2ExcHZaREpKZUZSWVpFNVhSVFZWV1cxek1WWnNjRmRoUlRWT1ZtMTRWMVl5Y3pWaFYwcFhVbXBPWVZaV1dsUldWRVpQVTBaV2NWSnNaRTVpYXpCNFZsWlNRazVXU25SVFdIQnBVbTFTV0ZwWE1UQk9WbHBYVld0MFZrMXJOWGxVYTJoelZsZEtjMk5HUmxaaVIwMHhWR3hhWVdNeVJrWmFSazVUVFVaWk1sWnJaREJrTVZWNFUxaHNWbUZyTlZoVVZWcDNVakZ3UlZKck5XeFdhM0I2V1ZWYWIxWXdNWEpOU0dSWFZrVndNMVY2Um1GWFJsSjFVMnhXYVdFeGNIbFhWekUwWkRKR1IxZHJaR2hTVlRWV1ZtcEdTMlZXV2toTlZFSm9WbXR3V2xaWGRHdFhiVXAxVVc1YVZtVnJSalJaZWtaaFpGZEdTR0ZHWkU1TlZWa3dWbXRTUTFZeVVuUldiR1JYVjBoQ1dGbHJXa3RpTVd4MFRWWk9UbFpzYkRWYVZWVTFZa1phY2sxVVVsZFNiRXBRV1ZSS1MyTXlUa2hTYkZwT1ZqRktlVlpzV21GWlZrcFhVbTVTVDFac1NsaFVWM0J5VFd4a2NsVnJjRTVXYlhoWVZsZHdZVlV4V2tkVGJGWmFWak5TTTFacVJuSmtNa1pJVW14T1UySnJTa3BYVkU1M1VUSkdTRk51VWxaaGExcFlWRmMxYjFWR1VuSlhhelZzVW10YWVWWlhNVWRWTURCM1RWUk9WMUpXU2xCWlZFcFRWakZ3U1ZSc1ZtbGlSWEJTVmxkNGFrNVZOVmRYYms1b1VsaFNXRlZxUW1GVFZsVjRWV3hPVjFJd1dubFpNRnBoVjBkR2NsZHNRbHBOVjFKVFdsWlZNVmRHU25OWGF6Vm9ZVEZ3VFZac1dsTlVNVkY1VTI1T1ZtSnJOVlpaYlRFMFkyeFNXR05HWkdoU2JIQldWVEZvYjFaV1NuTldhazVXVFdwV1ZGWXlNVWRqYXpWV1pFWm9hV0pyU25oV1JsSkNaVWRPZEZScldtcFNhelZQV1cweGEwNUdXbFZTYlVaU1RWVndTRmt3Vm05aVJrcFpVV3hTV21KR2NFeFdSM2hYVmxaR2NtUkhhRmRXUjNoSVYxZHdUMk14V1hoYVJWcFlZa1phWVZadWNGZGpiSEJIVjIxMGFrMVdTakZXVjNoVFZqRmFWVlpVU2xkTmFsWXpXV3BHVG1Rd01WbFViRnBwWW10S2RsWnRjRTlWTWxKWFYxaHNUbE5IVWxCV2FrWmhWakZzY2xkclpGWmhla1o2VlRJMWQxWnRWbkpYYm5CYVlsaG9lbFpxUm5OT2JFWjBVbTE0YVZZeWFFNVdhMXByVGtaWmQwMVZaRmhpYTFwVVdXeGtORlpzVWxoT1ZVNU9Za2hDUjFac2FHOVdNREZGVW10d1ZsWXpVVEJXYWtwTFkyc3hTV0pHWkZOU1dFSTFWMnRTUzFZeFNYbFNhMlJXWWtWd1dGVnFUbTlVUmxwWVRVUkdWVTFzU25wV1YzaHJWbTFHTmxac1FsZGlSa3BZV2xWYWMxZFhUa1pVYkZacFZteHZkMWRZY0U5aE1WcFlVbGhvVkdFelVsWlphMXBYVGtacmVGZHJPVmRXYkVwNVZERmFhMVV3TVVWUmFsSlhZVEZ3Y2xwRVJtdGphekZaVTJzMVYxSnVRbGRXYWtKcllqRlJlR05HYUU1V1ZGWnpWbTE0UzFkc2EzZFdibVJvWVhwR1NWcFZWbUZXTVVwMFlVaHdWMUpGV21oVmJYaGhZMVpXYzFac2FGUlNWWEJSVm10YVYxWnJNVlpPV0U1VFlrZDRjRlZzVW5OV1JsWjBaVVphYkZKc2NFaFdNbmhQVlRKR05sSnViRlZpUmtwb1ZsUkdZVkp0VGtaUFYwWlhUVEZLTlZaR1ZtRlZNazV6Vm01S1ZHRjZiRlJXYTFwaFRrWmFjMWR0T1ZKTlZrcFlWV3hvYTFVeVNuTmpSbHBhWWtkb1JGcFdXbmRXYkZwVlZteGtWMkpyUlhsV1ZtUjZUbFpWZUZOWWJGWldSVXBZVlc1d1YwMHhWbGhqTTJoVFlYcFdXRlp0ZUZkV01VcFpVV3hHVjJKWVVuSlpWekZYWXpGV2MxcEdXbWxTV0VKUlZsZDRVMk14V25OYVNGWnFVbFUxVmxSV1duZFRWbHBJWTBaa1YxWlVSa2RVYTJoM1YwZEdjbEpxVWxWaVdHaElWbXhhVDJOck9WaGxSMnhYVFZWV05GWnRkR0ZaVmxWNVZHNUtUMVpXY0ZGV2JYaGhWVEZaZDFwRVVsWlNiWGhaV2xWYVlXSkhTa2RTYWs1WFZqTlNhRlpIYzNoU1ZscHlWMnhrVGxacmNGbFdhMk40VWpKTmVGTnNWbEppUmxwd1dXMTBTMVZHWkhOWGJGcFBWbXR3U1ZWc2FIZFdWMFY2VVd4T1ZWWjZSbE5VVlZwaFVqRmtjMVJzV21sV2ExbDZWa1JHVjFZeGJGaFdia3BVWVhwc1lWWnFUbTlXTVhCWFYyeE9WMDFXV25wVk1uTXhZVWRXY2xkc1pGZFdNMmhVVlZSR1UyUkdXblZUYld4VFVteHdWMWRYTURGUk1WVjRWV3hvYWsweWFGQlphMXBoVm14V1dHTjZSbWxTTUhCWlZsWm9kMVl4U2taVGJFSlZWa1ZhTWxwRVFYaFdWbVJ6WTBkNGFFMHdTbEpXTVZwclpXMVJlVlpyWkZaaWJFcHhWV3hhZDFReFduTlZhMlJUWWtaYVdGWlhlR3RXVlRGV1kwVm9WMkpIYUROV2JGcHJVMVpHY2xwSFJsZE5NREUwVm0xNFlWUXhaRmRqUlZwUVZqSm9iMXBYZEVwTlJscHlWV3RrVmsxVmNEQldiVFZMVkRGa1JsTnRPVlZXUlVWNFZYcEdWMVpXVG5Sa1JUbFRZa1Z3V1Zac1pEQmtNVkY0VjJ4V2FWSnNjRmRXYlhoTFZFWmFWbGRyZEZOTlZYQjRWbTE0VjFZeVNrWmpSbEpYVWpOb1YxcFZXa3RXTVZwellrZG9VMVpIZUhwWFZsSkxaVzFSZUZkcmFFNVhSMUpXV1Zod1EwMHhVbk5oUldSWFZteHdSMVJzV2tkV01rVjRZMFZrVlZaV2NGTmFSRVpUWTFaU2RGSnJOVk5TVmxZMVZtcEtkMVF5UlhoVmJsSlRZbXhLY1ZSVVNsTldiR3gwVGxWT1YwMVlRbGxaZWs1dlZURmFjbU5HV2xaV00yaEVWbXBHU21Rd01VbGFSbVJUWWxaSmVsWldaSHBOVmtweVRWWldXR0pWV2s5VVZ6VnZWVlprVjFwRVVtcGlWbHA2Vmxab2IxWnRTa2xSYXpsV1lrWktlbFZzV25OV1ZsSnlUMVpTYVZKV1dURldNV2gzVWpGYVYxZFljRkpoTURWb1ZtcE9UbVZHVm5OWGJYUlhWbXR3ZWxWWGVHdFZNVnBHVW1wT1YyRXhjR2hXVkVwTFkyczFWbGRyTlZoU1ZuQldWbXBDYjFFeFVYaFdia3BYVjBkb1VGVnRlRXRYYkd0M1ZtNWtWVTFXY0ZaV2JYUmhWakZLZEdGRVRsZFdWbkJNVlRCYWMwNXNSbk5XYkU1WFZrWmFSVll5ZUdGV2JWWkdUbGhPVTJKR1dsVlpXSEJ6WWpGc1ZWSnNaRTVOVm5CR1ZXMTRUMVp0UmpaV2EyUlZZa2RvU0ZaVVJtdFRSMVpIVW14YWFFMXNTbmhXUmxwaFZURmtXRlJyV2s5V00xSlBWV3RXWVU1R1duTlhiRTVTVFZaS2VsWXlkRzlaVmtwSFUyeGFWMkZyTlVSVmFrWlRWMGRTUlZGc1pGTk5WVmt4VmpKMFYwNUdWWGhUV0hCVllXeHdXRlZxVG05V1JscElZek5vVTJGNlJsZFdiWGhMVjBaT1JsTnNTbGRXTTFKWVZtMXplRkl4Vm5WVGJGcHBVMFZLVUZaR1ZsTlNNbFpIWWtac2FsSlViRlpVVmxwM1UxWmFSMVZyWkZkV01GWTBWVEo0YTFaV1drWlNXR2hWWWxob1NGWXhaRXRTYXpsWVpVZG9UbUp0WTNsV2JGSkhWakZWZUZadVNrNVhSWEJRVm0xNFlWZFdXbkpXYTNSV1VtNUNXVlJXYUhkVWJFcHpVMnhzV2sxR1ZYaFdSRVpMVmxaYVZWTnNaRk5pVjJnMlZteFdZVmxXU25OWGJHeHFVbXhhYjFSV1ZuZE5WbVJ6VjJ4YVRsWnNjRmhaYTFKaFZtMUdObFpzYUZaaVIxRjNWRzE0WVZJeFduTlViRnBwVmxSV1NWZFdWbUZXTVZKelUydG9iRkpZVW1GV2JuQkNUVlphUlZKcldteFdNSEJIVlRJeFIxUnNTWHBoUlZwWFZqTlNkbGxVU2xOV01YQkpWR3hXYUdFd2NGSldWM2hxVGxVMVYxZHVTbWhTYkhCeFdWUk9VazFzYkhKaFJVNWFWakJXTTFSc2FFdFhiR1JKVVc1S1dtSkhVbFJhUmxwclpFWktjMWRzWkd4aVdHaE5WakZhVTFNeFZYZE5XRTVwVTBWYVZWbHJWbmRoUmxKWFlVVk9UbEp1UWxkWGEyTTFWMnhaZUZkc1pGZGlWRlpFVm10VmVGWnNXbkZVYkZwc1lURnZlbFpxUW10V01VNVlWR3RvVTJKSFVsUmFWekUwVlZaYVYxVnJkR3BOVlZwNldXcE9hMkZXU2xkVGJGSldUVWRTVkZSV1dscGxWMDQyVldzMVYxWkdWWGxXTVZKUFl6RmFkRlp1VW1oU1dHaFdWbXBPYjFkR2NFVlRiRTVxWWtaS01GcEZaSE5WTVdSSFVtcFdXR0V4U2t0VVZscHpWakZhV1dOSGFGTlhSMmhVVmtaV2IxRXhaSE5hUmxaVVYwZFNVRlpxUmt0WFJtdDNWV3hrVm1KVmNGWlZiVFZEVm0xR2NsTnRhR0ZTYkhCUVZtcEdVMk50VGtoalJrcE9ZWHBWZWxaWWNFZFNhemxYVVd4U1drMHdTazFXVmxGM1QxVndOV0V6UVc1TFUyczlKeWtwJykp'))
```

## Unpacking

In the first step, I decided to drop the base64 content into CyberChef, but in the next iteration there was another layer of base64 encoded code. So, I wrote a simple script in py that will do the "dirty work".

```python
from base64 import b64decode

if __name__ == '__main__':
    with open('python_freak.py', 'r') as f:
        data = f.read()

    layer_count = 0
    while 'b64decode' in data:
        layer_count += 1
        data = data.split('\'')[1]
        data = b64decode(data).decode()

    print(f'{layer_count=}')
    with open('python_freak_unpacked.py', 'wb') as f:
        f.write(str.encode(data))
```

After 7 iterations, the final form appeared!

```python
𝓿 = 𝓉𝔂𝓹𝓮("m", (), {"__init__": lambda 𝔞, ｂ: 𝓈𝔢ｔ𝒶𝓽ｔ𝓻(𝒶, "b", 𝔟), "s": lambda 𝒶, ｂ: 𝔳(𝔟(𝔞.𝒷).𝓫)})
𝒸 = 𝔱𝓎𝓹𝓮("l", (), {"__init__": lambda 𝔞, 𝔟: 𝔰𝑒𝓽ａ𝓉𝓉𝔯(ａ, "b", 𝓫), "d": lambda ａ, 𝔟: 𝔞 if 𝓋(ａ.𝒷).ｓ(𝒷).𝔟 != ｂ(ａ.𝔟).𝓫 else ｂ(𝔞.𝔟)})
𝔪 = lambda 𝔵: 𝔠(f"{𝓵𝔦𝓈𝔱(𝓏𝔦𝔭([𝓁(𝔦 + 5*𝔦) for ｉ in 𝔁], [𝔩(𝔦 + 𝓲) for 𝔦 in 𝔵][::-1]))[::-1] = }")
𝔫 = lambda ｘ: 𝒸("{x[';]:=^450}".forｍ𝒶𝓽(𝔵={'''';''': 𝔵}))
𝔩 = lambda 𝔁: (12648430 ^ (𝔵 + 3735928559)) * 3862272608
𝓀 = 𝓲ｎ𝔭𝔲𝓽("Insert your flag: ")
𝓈 = [(lambda 𝔷, ｆ, ｘ, 𝔫: 𝓍 if 𝓷 == 0 else 𝔣(𝔷, 𝒻, (lambda 𝓶, 𝓃: (lambda 𝓍: 𝒾ｎ𝔱(𝔁[ｌ𝔢𝔫(𝔵)-59:], 2) + 𝒾𝓷𝓽(𝔵[:𝔩𝓮ｎ(𝔁)-59], 2) if 𝔩ｅ𝔫(𝔁)-59 > 0 else 𝔦ｎ𝓽(𝔵,2))(ｂ𝒾𝔫(𝓶 * 𝓷)))(𝔁, 𝔃), ｎ - 1))(𝔰𝔲𝓂((ｏ𝔯𝔡(𝔨[𝓲+𝒿]) << 𝒿*7) if ｉ+ｊ < ｌ𝔢𝔫(𝓴) else (127 << 𝒿*7) for 𝔧 in 𝓇𝔞ｎ𝑔𝔢(8)), (lambda 𝔷, 𝓯, ｘ, 𝔫: ｘ if 𝔫 == 0 else ｆ(𝔷, 𝓯, (lambda 𝔫, 𝔪: (lambda 𝔵: ｉｎ𝔱(ｘ[𝓁ｅ𝔫(𝓍)-59:], 2) + 𝓲ｎｔ(𝔁[:𝓁𝑒𝔫(𝔵)-59], 2) if 𝓵𝑒𝔫(𝓍)-59 > 0 else 𝒾ｎ𝓽(ｘ,2))(𝓫𝔦𝔫(𝓷 * 𝔪)))(𝔁, 𝔷), 𝔫 - 1)), 1, 420_69) for 𝔦 in 𝓇ａ𝓷𝑔ｅ(0, 𝓁𝔢𝓷(𝔨), 8)]
if 𝓬(ｓ).𝔡(𝔪).𝒹(𝔫).ｂ == "=𝓵𝔦𝓈𝔱(𝓏𝔦𝔭([𝓁(𝔦 + 5*𝔦) for ｉ in 𝔁], [𝔩(𝔦 + 𝓲) for 𝔦 in 𝔵][::-1]))[::-1] = [(13969439442922757926633137632, 3251133470245911671632840864), (6919844817045365871489845728, 3067821989026578174692487328), (11408842561461143227463443808, 3766356150094573135206359136), (11299068421490417286376379488, 3802947530149782083826679648), (9203465938188223031329433888, 2306614948612889330244181216), (9753400381846729757945770272, 4656479823873291748257812704)]==":
    𝔭𝓻ｉ𝓷ｔ("Good flag!")
else:
    𝓹ｒ𝔦𝔫𝔱("Wrong flag!")
```

Uhuhuh, so it's time to...

## Deobfuscation - Form #1

The code is written as unicode, which makes it much more difficult to understand. I used the normalize function from the unicodedata package and broke up the longest lambda to improve readability.

```python
from unicodedata import normalize

if __name__ == '__main__':
    with open('python_freak_unpacked.py', 'rb') as f:
        data = f.read().decode("UTF-8")

    data = normalize('NFKD', data)
    data = data.replace('(lambda', '\n(lambda')
    print(data)
```

And then I got the base code:

```python
v = type("m", (), {"__init__": lambda a, b: setattr(a, "b", b), "s": lambda a, b: v(b(a.b).b)})
c = type("l", (), {"__init__": lambda a, b: setattr(a, "b", b), "d": lambda a, b: a if v(a.b).s(b).b != b(a.b).b else b(a.b)})
m = lambda x: c(f"{list(zip([l(i + 5*i) for i in x], [l(i + i) for i in x][::-1]))[::-1] = }")
n = lambda x: c("{x[';]:=^450}".format(x={'''';''': x}))
l = lambda x: (12648430 ^ (x + 3735928559)) * 3862272608
k = input("Insert your flag: ")
s = [
(lambda z, f, x, n: x if n == 0 else f(z, f,
(lambda m, n:
(lambda x: int(x[len(x)-59:], 2) + int(x[:len(x)-59], 2) if len(x)-59 > 0 else int(x,2))(bin(m * n)))(x, z), n - 1))(sum((ord(k[i+j]) << j*7) if i+j < len(k) else (127 << j*7) for j in range(8)),
(lambda z, f, x, n: x if n == 0 else f(z, f,
(lambda n, m:
(lambda x: int(x[len(x)-59:], 2) + int(x[:len(x)-59], 2) if len(x)-59 > 0 else int(x,2))(bin(n * m)))(x, z), n - 1)), 1, 420_69) for i in range(0, len(k), 8)]
if c(s).d(m).d(n).b == "=list(zip([l(i + 5*i) for i in x], [l(i + i) for i in x][::-1]))[::-1] = [(13969439442922757926633137632, 3251133470245911671632840864), (6919844817045365871489845728, 3067821989026578174692487328), (11408842561461143227463443808, 3766356150094573135206359136), (11299068421490417286376379488, 3802947530149782083826679648), (9203465938188223031329433888, 2306614948612889330244181216), (9753400381846729757945770272, 4656479823873291748257812704)]==":
    print("Good flag!")
else:
    print("Wrong flag!")
```

At first glance, you can see 2 classes - 'c' and 'v'. 3 simple functions ('m', 'n', 'l') and 1, which will probably be the greatest difficulty - 's'. So it's time to rewrite the code into a more understandable form.

## Deobfuscation - Form #2

I rebuilt both classes, changed 3 simple lambdas to functions, extracted and grouped expressions from the longest lambda.

```python
class v:
    def __init__(self, b) -> None:
        self.b = b

    def s(self, b):
        return v(b(self.b).b)

class c:
    def __init__(self, b) -> None:
        self.b = b

    def d(self, b):
        return self if v(self.b).s(b).b != b(self.b).b else b(self.b)

def l(x):
    x += 0xDEAD_BEEF
    x ^= 0xC0FFEE
    x *= 0xE635_9A60
    return x

def m(x):
    l1 = [l(6 * j) for j in x]
    l2 = [l(2 * j) for j in x]
    l2 = reversed(l2)
    result = list(zip(l1, l2))

    return c(f"list(zip([l(i + 5*i) for i in x], [l(i + i) for i in x][::-1]))[::-1] = {result}")

def n(x):
    return c("{x[';]:=^450}".format(x={'''';''': x}))

k = input("Insert your flag: ")
op1 = lambda x: int(x[len(x)-59:], 2) + int(x[:len(x)-59], 2) if len(x)-59 > 0 else int(x,2)
op2 = lambda m, n: op1(bin(m * n))
op3 = lambda z, f, x, n: x if n == 0 else f(z, f, op2(x, z), n - 1)
op4 = lambda i: sum((ord(k[i+j]) << j*7) if i+j < len(k) else (127 << j*7) for j in range(8))
s = [op3(op4(i), op3, 1, 420_69) for i in range(0, len(k), 8)]

if c(s).d(m).d(n).b == "=list(zip([l(i + 5*i) for i in x], [l(i + i) for i in x][::-1]))[::-1] = [(13969439442922757926633137632, 3251133470245911671632840864), (6919844817045365871489845728, 3067821989026578174692487328), (11408842561461143227463443808, 3766356150094573135206359136), (11299068421490417286376379488, 3802947530149782083826679648), (9203465938188223031329433888, 2306614948612889330244181216), (9753400381846729757945770272, 4656479823873291748257812704)]==":
    print("Good flag!")
else:
    print("Wrong flag!")
```

At this level, I noticed that a next simplifications can be made.

## Deobfuscation - Form #3

I noticed that the condition in class 'c' will never be met, so class v is redutant. I also wrote the formatting of the caption corresponding to the lambda 'n' in a clearer way. In addition, it is worth mentioning the snekay implementation of binary modulo, which is done in the 'op1' function, leading to the use of the op2 function as a wrapper, which only multiplies the input args and changes result to a binary representation. The 'op3' function recursively performs the 'op2' operation 42069 times. And function 'op4' is responsible for preparing the input data, it encodes ASCI characters.

```python
class c:
    def __init__(self, b) -> None:
        self.b = b

    def d(self, b):
        return b(self.b)

def l(x):
    x += 0xDEAD_BEEF
    x ^= 0xC0FFEE
    x *= 0xE635_9A60
    return x

def m(x):
    l1 = [l(6 * j) for j in x]
    l2 = [l(2 * j) for j in x]
    l2 = reversed(l2)
    result = list(zip(l1, l2))

    return c(f"list(zip([l(i + 5*i) for i in x], [l(i + i) for i in x][::-1]))[::-1] = {result}")

def n(x):
    return c(x.center(450, '='))

k = input("Insert your flag: ")

def op1(x):
    return x % (2**59 - 1)

def op2(m, n):
    return op1(m * n)

def op3(z, f, x, n):
    if n == 0:
        return x

    return f(z, f, op2(x, z), n - 1)

def op4(i):
    result = 0

    for j in range(8):
        if i+j < len(k):
            result += ord(k[i+j]) << j*7
        else:
            result += (0x7F << j*7)

    return result

def s():
    result = list()

    for i in range(0, len(k), 8):
        tmp_result = op3(op4(i), op3, 1, 420_69)
        result.append(tmp_result)

    return result

s = s()
if c(s).d(m).d(n).b == "=list(zip([l(i + 5*i) for i in x], [l(i + i) for i in x][::-1]))[::-1] = [(13969439442922757926633137632, 3251133470245911671632840864), (6919844817045365871489845728, 3067821989026578174692487328), (11408842561461143227463443808, 3766356150094573135206359136), (11299068421490417286376379488, 3802947530149782083826679648), (9203465938188223031329433888, 2306614948612889330244181216), (9753400381846729757945770272, 4656479823873291748257812704)]==":
    print("Good flag!")
else:
    print("Wrong flag!")
```

Let's try to simplify it even more.

## Deobfuscation - Form #4

I decided to get rid of the 'c' class because its purpose was reversing the order in the calling functions/lambdas. Put the function code 'op_1', 'op_2' and 'op3' into the function body 'op_4'. In addition, the 'op3' functions was simplified by replacing the recursion with a loop. 

```python
def l(x):
    x += 0xDEAD_BEEF
    x ^= 0xC0FFEE
    x *= 0xE635_9A60
    return x

def m(x):
    l1 = [l(6 * j) for j in x]
    l2 = [l(2 * j) for j in x]
    l2 = reversed(l2)
    result = list(zip(l1, l2))

    return f"list(zip([l(i + 5*i) for i in x], [l(i + i) for i in x][::-1]))[::-1] = {result}"

def n(x):
    return x.center(450, '=')

k = input("Insert your flag: ")

def op4(k, i):
    result = 0

    for j in range(8):
        if i+j < len(k):
            result += ord(k[i+j]) << j*7
        else:
            result += (0x7F << j*7)

    return result

def s(user_inp):
    result_arr = []
    for idx in range(0, len(user_inp), 8):
        n = op4(user_inp, idx)
        m = 1
        cnt = 420_69
        while cnt > 0:
            cnt -= 1
            m = (m * n) % (2**59 - 1)

        result_arr.append(m)

    return result_arr

s = s(k)
if n(m(s)) == "=list(zip([l(i + 5*i) for i in x], [l(i + i) for i in x][::-1]))[::-1] = [(13969439442922757926633137632, 3251133470245911671632840864), (6919844817045365871489845728, 3067821989026578174692487328), (11408842561461143227463443808, 3766356150094573135206359136), (11299068421490417286376379488, 3802947530149782083826679648), (9203465938188223031329433888, 2306614948612889330244181216), (9753400381846729757945770272, 4656479823873291748257812704)]==":
    print("Good flag!")
else:
    print("Wrong flag!")
```

Let's try to simplify it a little more!

## Deobfuscation - Form #5

Let's see the final form of the code, which performs the following steps:
- encodes each 8 ASCI characters into 1 number.
- encrypts the generated number using RSA (exp=42069, mod=2**59 - 1, thanks to crypto guys from the team!).
- multiplies the encrypted number once by 6 times, once by 2 times and performs addition, xor and multiplication operations with constants each time. On the basis of these operations, it creates 2 lists, of which list #2 is written in reverse order.
- prints the result list by filling padding on both sides with the '=' sign. 

```python
def l(x):
    x += 0xDEAD_BEEF
    x ^= 0xC0FFEE
    x *= 0xE635_9A60
    return x

def m(x):
    l1 = [l(6 * j) for j in x]
    l2 = [l(2 * j) for j in x]
    l2 = reversed(l2)
    result = list(zip(l1, l2))

    return f"list(zip([l(i + 5*i) for i in x], [l(i + i) for i in x][::-1]))[::-1] = {result}"

def n(x):
    return x.center(450, '=')

k = input("Insert your flag: ")

def prepare_input(k, i):
    result = 0

    for j in range(8):
        if i+j < len(k):
            result += ord(k[i+j]) << j*7
        else:
            result += (0x7F << j*7)

    return result

def rsa_encrypt(n):
    m = (n**420_69) % (2**59 - 1)
    return m

def s(user_inp):
    result_arr = []
    for idx in range(0, len(user_inp), 8):
        n = prepare_input(user_inp, idx)
        m = rsa_encrypt(n)
        result_arr.append(m)

    return result_arr

s = s(k)
if n(m(s)) == "=list(zip([l(i + 5*i) for i in x], [l(i + i) for i in x][::-1]))[::-1] = [(13969439442922757926633137632, 3251133470245911671632840864), (6919844817045365871489845728, 3067821989026578174692487328), (11408842561461143227463443808, 3766356150094573135206359136), (11299068421490417286376379488, 3802947530149782083826679648), (9203465938188223031329433888, 2306614948612889330244181216), (9753400381846729757945770272, 4656479823873291748257812704)]==":
    print("Good flag!")
else:
    print("Wrong flag!")
```

So it's high time to reverse the code!

## Reversed code

In order to obtain the flag, I started by extracting the array from the character string that is expected - n_reversed. Then, I reconstructed the numbers that are encrypted with RSA - m_reserved. I skipped the implementation of recovering the private key, instead used the web services - reverse_rsa_pubkey and rsa_decrypt_helper. The last operation was to reverse the character encoding - prepare_input_reversed.

```python
import ast

def l_reversed(x):
    x //= 0xE635_9A60
    x ^= 0xC0FFEE
    x -= 0xDEAD_BEEF
    
    return x

def m_reserved(x):
    result = []
    x_len = len(x) - 1

    for x_idx, x_elem in enumerate(x):
        l1, l2 = x_elem[0], x[x_len - x_idx][1]
        l1 = l_reversed(l1) // 6
        l2 = l_reversed(l2) // 2

        assert l1 == l2
        result.append(l1)

    return result

def n_reversed(x):
    list_as_str = x.split('=')[2]
    extracterd_list = ast.literal_eval(list_as_str)
    return extracterd_list

def prepare_input_reversed(k_reversed):
    out_str_chunk = ""

    for _ in range(8):
        curr_leter = k_reversed & 0x7F 
        if curr_leter == 0x7F:
            break

        k_reversed >>= 7
        out_str_chunk += chr(curr_leter)
    
    return out_str_chunk

def rsa_decrypt(n):
    m = (n ** 331357922622589629) % (2**59 - 1)
    return m

#helpers - no need to implement crypto
def reverse_rsa_pubkey():
    n = 2**59 - 1
    #http://www.factordb.com/index.php?query=576460752303423487 <- from n
    p, q = 179951, 3203431780337
    phi = (p - 1) * (q - 1)
    e = 420_69
    #https://www.extendedeuclideanalgorithm.com/calculator.php?mode=1&a=576457548871463200&b=42069#num <- from phi and e 
    x = -245099626248873571
    d = x % phi
    return p, q, d, n, e # -> (179951, 3203431780337, 331357922622589629, 576460752303423487, 42069)

def rsa_decrypt_helper(n):
    #https://www.dcode.fr/rsa-cipher <- used to decrypt
    resolved_hashes = {602816046677732626: 72057594037894484, 298608509588668418:29777244051795950, 492319924258423028: 27480917037021748, 487582897162708488:53783139771219121, 397152439243482845: 61193963799592159, 420883477912243645: 67205173222290672}
    return resolved_hashes[n]

looked_str = "=list(zip([l(i + 5*i) for i in x], [l(i + i) for i in x][::-1]))[::-1] = [(13969439442922757926633137632, 3251133470245911671632840864), (6919844817045365871489845728, 3067821989026578174692487328), (11408842561461143227463443808, 3766356150094573135206359136), (11299068421490417286376379488, 3802947530149782083826679648), (9203465938188223031329433888, 2306614948612889330244181216), (9753400381846729757945770272, 4656479823873291748257812704)]=="
encrypted_input_data = m_reserved(n_reversed(looked_str))
decrypted_input_data = str.join("", map(lambda x: prepare_input_reversed(rsa_decrypt_helper(x)), reversed(encrypted_input_data)))
print(decrypted_input_data)
```

Combining the above steps, I obtained the flag I was looking for:

```
ping{W0w_y0U_rYl1y_g0oD_4t_Py7h0n_c0nGr4Tz}
```
